
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

using namespace indri::index;

//
// _fetchMatchingInvertedLists
//

void IndexWriter::_fetchMatchingInvertedLists( greedy_vector<WriterInvertedList*>& lists, invertedlist_pqueue& queue ) {
  WriterInvertedList* first = queue.top();
  lists.push_back( first );
  const char* firstTerm = first->iterator->currentEntry()->termData->term;
  queue.pop();

  while( !strcmp( firstTerm, queue.top()->iterator->currentEntry()->termData->term ) ) {
    lists.push_back( queue.top() );
    queue.pop();
  }
}

//
// _pushInvertedLists
//

void IndexWriter::_pushInvertedLists( greedy_vector<WriterInvertedList*>& lists, invertedlist_pqueue& queue ) {
  for( int i=0; i<lists.size(); i++ ) {
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

  for( iter = lists.begin(); iter != lists.end(); ++iter ) {
    indri::index::DocListFileIterator::DocListData* listData = (*iter)->iterator->currentEntry();
    ::termdata_merge( termData, listData->termData, _fieldCount );
  }

  _termDataBuffer.clear();
  RVLCompressStream stream( _termDataBuffer );

  stream << termData->term;
  ::termdata_compress( stream, termData, _fieldCount );

  int dataSize = stream.dataSize();
  _output.write( &dataSize, sizeof(int) );
  _output.write( stream.data(), stream.dataSize() );
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
  const skipLength = 1<<12; // 4k
  int documentsWritten = 0;

  const float topdocsFraction = 0.01;
  bool hasSkips = termData->corpus.totalCount > 10000;
  bool hasTopdocs = termData->corpus.documentCount > 1000;
  int topdocsCount = hasTopdocs ? int(termData->corpus.totalCount * 0.01) : 0;
  int topdocsSpace = hasTopdocs ? topdocsCount*3*sizeof(DocListIterator::TopDocument) + sizeof(int) : 0;

  // write a control byte
  char control = (hasSkips ? 1 : 0) | (hasTopdocs ? 2 : 0);
  _output.write( &control, 1 );

  UINT64 initialPosition = _output.tell();

  // leave some room for the topdocs list
  if( hasTopdocs ) {
    _output.write( &topdocsCount, sizeof(int) );
    _output.seek( topdocsSpace + _output.tell() );
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

      if( listBuffer.position() > skipLength && hasSkips ) {
        // time to write in a skip
        int skipLength = listBuffer.position();
        int skipDocument = documentData->document;

        _output.write( &skipDocument, sizeof(int) );
        _output.write( &skipLength, sizeof(int) );
        _output.write( listBuffer.front(), listBuffer.position() );
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
    }
  }

  // write in the final skip info
  if( hasSkips ) {
    int skipDocument = MAX_INT32;
    int skipLength = MAX_INT32;
    _output.write( &skipDocument, sizeof(int) );
    _output.write( &skipLength, sizeof(int) );
  }

  _output.write( listBuffer.front(), listBuffer.position() );
  listBuffer.clear();
  UINT64 finalPosition = _output.tell();

  if( hasTopdocs ) {
    _output.seek( initialPosition );
    _output.write( &topdocsCount, sizeof(int) );

    // write these into the topdocs list in order from smallest fraction to largest fraction,
    // where fraction = c(w;D)/|D|
    while( topdocs.size() ) {
      DocListIterator::TopDocument& topDocument = topdocs.top();
      
      _output.write( &topDocument.document, sizeof(int) );
      _output.write( &topDocument.count, sizeof(int) );
      _output.write( &topDocument.length, sizeof(int) );
      topdocs.pop();
    }

    _output.seek( finalPosition );
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

  for( int i=0; i<indexes.size(); i++ )
    invertedLists.push( new WriterInvertedList( indexes[i]->docListFileIterator() ) );

  greedy_vector<WriterInvertedList*> current;
  indri::index::TermData* termData = ::termdata_create( _fieldCount );

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
  _output.flush();
  _outfile.close();
}

