
//
// IndexWriter
//
// 18 November 2004 -- tds
//
// Writes out a single index from one or more indexes.
//

#include "indri/IndexWriter.hpp"
#include "indri/Index.hpp"
#include "indri/DocListFileIterator.hpp"
#include <vector>
#include <queue>
#include "indri/greedy_vector"
#include "indri/VocabularyMap.hpp"
#include "indri/Path.hpp"

using namespace indri::index;

//
// IndexWriter constructor
//

IndexWriter::IndexWriter( int fields ) :
  _fieldCount(fields)
{
}

//
// write
//

void IndexWriter::write( indri::index::Index& index, const std::string& path ) {
  Path::create( path );

  std::string frequentStringPath = Path::combine( path, "frequentString" );
  std::string infrequentStringPath = Path::combine( path, "infrequentString" );
  std::string frequentIDPath = Path::combine( path, "frequentID" );
  std::string infrequentIDPath = Path::combine( path, "infrequentID" );
  std::string documentLengthsPath = Path::combine( path, "documentLengths" );
  std::string documentStatisticsPath = Path::combine( path, "documentStatistics" );
  std::string invertedFilePath = Path::combine( path, "invertedFile" );
  std::string directFilePath = Path::combine( path, "directFile" );

  // infrequent stuff
  _infrequentTerms.idMap = new Keyfile();
  _infrequentTerms.idMap->create( infrequentIDPath );
  _infrequentTerms.stringMap = new Keyfile();
  _infrequentTerms.stringMap->create( infrequentStringPath );

  // frequent stuff
  _frequentTerms.idMap = new Keyfile();
  _frequentTerms.idMap->create( frequentIDPath );
  _frequentTerms.stringMap = new Keyfile();
  _frequentTerms.stringMap->create( frequentStringPath );

  // stats, inverted file, direct file
  _documentStatistics.create( documentStatisticsPath );
  _documentLengths.create( documentLengthsPath );
  _invertedFile.create( invertedFilePath );
  _directFile.create( directFilePath );

  _invertedOutput = new SequentialWriteBuffer( _invertedFile, 1024*1024 );

  std::vector<indri::index::Index*> indexes;
  indexes.push_back( &index );
  
  _writeInvertedLists( indexes );

  // close infrequent
  _infrequentTerms.idMap->close();
  delete _infrequentTerms.idMap;
  _infrequentTerms.stringMap->close();
  delete _infrequentTerms.stringMap;

  // close frequent
  _frequentTerms.idMap->close();
  delete _frequentTerms.idMap;
  _frequentTerms.stringMap->close();
  delete _frequentTerms.stringMap;

  // close stats
  _documentStatistics.close();
  _documentLengths.close();
  _invertedFile.close();
  _directFile.close();
}

//
// _fetchMatchingInvertedLists
//

void IndexWriter::_fetchMatchingInvertedLists( greedy_vector<WriterInvertedList*>& lists, invertedlist_pqueue& queue ) {
  lists.clear();

  WriterInvertedList* first = queue.top();
  lists.push_back( first );
  const char* firstTerm = first->iterator->currentEntry()->termData->term;
  queue.pop();

  while( queue.size() && !strcmp( firstTerm, queue.top()->iterator->currentEntry()->termData->term ) ) {
    lists.push_back( queue.top() );
    queue.pop();
  }
}

//
// _pushInvertedLists
//

void IndexWriter::_pushInvertedLists( greedy_vector<WriterInvertedList*>& lists, invertedlist_pqueue& queue ) {
  for( int i=0; i<lists.size(); i++ ) {
    lists[i]->iterator->nextEntry();

    if( lists[i]->iterator->finished() ) {
      delete lists[i];
    } else {
      queue.push( lists[i] );
    }
  }
}

//
// _writeStatistics
//

void IndexWriter::_writeStatistics( greedy_vector<WriterInvertedList*>& lists, indri::index::TermData* termData ) {
  greedy_vector<WriterInvertedList*>::iterator iter;
  ::termdata_clear( termData, _fieldCount );

  // find out what term we're writing
  strcpy( const_cast<char*>(termData->term), lists[0]->iterator->currentEntry()->termData->term );

  for( iter = lists.begin(); iter != lists.end(); ++iter ) {
    indri::index::DocListFileIterator::DocListData* listData = (*iter)->iterator->currentEntry();
    ::termdata_merge( termData, listData->termData, _fieldCount );
  }

  _termDataBuffer.clear();
  RVLCompressStream stream( _termDataBuffer );

  stream << termData->term;
  ::termdata_compress( stream, termData, _fieldCount );

  int dataSize = stream.dataSize();
  _invertedOutput->write( &dataSize, sizeof(int) );
  _invertedOutput->write( stream.data(), stream.dataSize() );
}

//
// _addInvertedListData
//
// Inverted list is:
//   termData (as written by _writeStatistics)
//   control byte -- hasSkips(0x1), hasTopdocs(0x2)
//   optional topdocs list: topdocsCount + (document/count/length)+
//   ( [skip: document/skipLength] (doc/positionCount/positions+)+ )
// MAX_INT32/MAX_INT32 signifies there's no more skips
// 
//

void IndexWriter::_addInvertedListData( greedy_vector<WriterInvertedList*>& lists, indri::index::TermData* termData, Buffer& listBuffer, UINT64& startOffset, UINT64& endOffset ) {
  greedy_vector<WriterInvertedList*>::iterator iter;
  const minimumSkip = 1<<12; // 4k
  int documentsWritten = 0;

  const float topdocsFraction = 0.01f;
  bool hasTopdocs = termData->corpus.documentCount > 1000;
  int topdocsCount = hasTopdocs ? int(termData->corpus.totalCount * 0.01) : 0;
  int topdocsSpace = hasTopdocs ? topdocsCount*3*sizeof(DocListIterator::TopDocument) + sizeof(int) : 0;

  // write a control byte
  char control = (hasTopdocs ? 0x01 : 0);
  _invertedOutput->write( &control, 1 );

  UINT64 initialPosition = _invertedOutput->tell();

  // leave some room for the topdocs list
  if( hasTopdocs ) {
    _invertedOutput->write( &topdocsCount, sizeof(int) );
    _invertedOutput->seek( topdocsSpace + _invertedOutput->tell() );
  }

  // maintain a list of top documents
  std::priority_queue<DocListIterator::TopDocument,
                      std::vector<DocListIterator::TopDocument>,
                      DocListIterator::TopDocument::less> topdocs;

  int lastDocument = 0;

  // for each matching list:
  for( iter = lists.begin(); iter != lists.end(); ++iter ) {
    indri::index::DocListFileIterator::DocListData* listData = (*iter)->iterator->currentEntry();
    DocListIterator* iterator = listData->iterator;
    Index* index = (*iter)->index;
    RVLCompressStream stream( listBuffer );

    while( !iterator->finished() ) {
      // get the latest entry from the list
      DocListIterator::DocumentData* documentData = iterator->currentEntry();

      // form a topdocs entry for this document
      DocListIterator::TopDocument topDocument( documentData->document,
                                                documentData->positions.size(),
                                                index->documentLength( documentData->document ) );


      // update the topdocs list
      topdocs.push( topDocument );
      while( topdocs.size() > topdocsCount )
        topdocs.pop();

      if( listBuffer.position() > minimumSkip ) {
        // time to write in a skip
        int skipLength = listBuffer.position();
        int skipDocument = documentData->document;

        _invertedOutput->write( &skipDocument, sizeof(int) );
        _invertedOutput->write( &skipLength, sizeof(int) );
        _invertedOutput->write( listBuffer.front(), listBuffer.position() );
        listBuffer.clear();

        // delta encode documents by batch
        lastDocument = 0;
      }

      // write this entry out to the list
      stream << documentData->document - lastDocument;
      stream << documentData->positions.size();
      lastDocument = documentData->document;

      int lastPosition = 0;

      for( int i=0; i<documentData->positions.size(); i++ ) {
        stream << (documentData->positions[i] - lastPosition);
      }

      iterator->nextEntry();
    }
  }

  // write in the final skip info
  int skipDocument = MAX_INT32;
  int skipLength = listBuffer.position();
  _invertedOutput->write( &skipDocument, sizeof(int) );
  _invertedOutput->write( &skipLength, sizeof(int) );

  _invertedOutput->write( listBuffer.front(), listBuffer.position() );
  listBuffer.clear();
  UINT64 finalPosition = _invertedOutput->tell();

  if( hasTopdocs ) {
    _invertedOutput->seek( initialPosition );
    _invertedOutput->write( &topdocsCount, sizeof(int) );

    // write these into the topdocs list in order from smallest fraction to largest fraction,
    // where fraction = c(w;D)/|D|
    while( topdocs.size() ) {
      DocListIterator::TopDocument& topDocument = topdocs.top();
      
      _invertedOutput->write( &topDocument.document, sizeof(int) );
      _invertedOutput->write( &topDocument.count, sizeof(int) );
      _invertedOutput->write( &topDocument.length, sizeof(int) );
      topdocs.pop();
    }

    _invertedOutput->seek( finalPosition );
  }
}

//
// _storeTermEntry
//

void IndexWriter::_storeTermEntry( IndexWriter::keyfile_pair& pair, indri::index::TermData* termData, INT64 startOffset, INT64 endOffset, int termID ) {
  _termDataBuffer.clear();
  RVLCompressStream stream( _termDataBuffer );
  
  stream << termData->term
         << termID;
  ::termdata_compress( stream, termData, _fieldCount );
  stream << startOffset
         << endOffset;

  pair.idMap->put( termID, stream.data(), stream.dataSize() );
  pair.stringMap->put( termData->term, stream.data(), stream.dataSize() );
}

//
// _storeFrequentTerms
//

void IndexWriter::_storeFrequentTerms() {
  // sort the _topTerms vector by term count
  std::sort( _topTerms.begin(), _topTerms.end(), top_term_entry::greater() );

  for( int i=0; i<_topTerms.size(); i++ ) {
    int termID = i+1;
    _storeTermEntry( _frequentTerms, _topTerms[i].termData, _topTerms[i].startOffset, _topTerms[i].endOffset, termID );
  }
}

//
// _storeMatchInformation
//

void IndexWriter::_storeMatchInformation( int sequence, indri::index::TermData* termData, UINT64 startOffset, UINT64 endOffset ) {
  // TODO: pick cutoff dynamically
  if( termData->corpus.documentCount > 10000 ) {
    top_term_entry entry;

    entry.termData = ::termdata_create( _fieldCount );
    ::termdata_merge( entry.termData, termData, _fieldCount );
    entry.startOffset = startOffset;
    entry.endOffset = endOffset;

    _topTerms.push_back( entry );
  } else {
    _storeTermEntry( _infrequentTerms, termData, startOffset, endOffset, sequence - _topTerms.size() );
  }
}

//
// writeInvertedLists
//

void IndexWriter::_writeInvertedLists( std::vector<indri::index::Index*>& indexes ) {
  
  // write a combined inverted list in vocabulary order
  // in the process, create a new list of termIDs from the old list
  
  std::priority_queue<WriterInvertedList*,
                      std::vector<WriterInvertedList*>,
                      WriterInvertedList::less> invertedLists;
  Buffer invertedListBuffer;

  UINT64 startOffset;
  UINT64 endOffset;

  // clear out the term buffer
  char term[Keyfile::MAX_KEY_LENGTH+1];
  term[0] = 0;
  int sequence = 1;

  for( int i=0; i<indexes.size(); i++ ) {
    invertedLists.push( new WriterInvertedList( indexes[i]->docListFileIterator(), indexes[i] ) );
  }

  greedy_vector<WriterInvertedList*> current;
  indri::index::TermData* termData = ::termdata_create( _fieldCount );
  char termBuffer[Keyfile::MAX_KEY_LENGTH+1] = {0};
  termData->term = termBuffer;

  for( int sequence = 1; invertedLists.size(); sequence++ ) {
    // fetch useful doc lists
    _fetchMatchingInvertedLists( current, invertedLists );

    // loop 1: merge statistics for term
    _writeStatistics( current, termData );

    // go through lists one by one, adding data to the final invlist, adding skips, etc.
    _addInvertedListData( current, termData, invertedListBuffer, startOffset, endOffset );

    // have to store the termData in a B-Tree (or something) for fast access later
    _storeMatchInformation( sequence, termData, startOffset, endOffset );

    // push back all doc lists with useful information
    _pushInvertedLists( current, invertedLists );
  }

  // at this point, we need to fill in all the "top" vocabulary data
  // into the keyfile

  ::termdata_delete( termData, _fieldCount );
  _invertedOutput->flush();
  delete _invertedOutput;
  _invertedFile.close();
}

