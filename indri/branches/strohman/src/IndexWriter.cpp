
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
#include "indri/Parameters.hpp"
#include "indri/DiskTermData.hpp"
#include "indri/TermBitmap.hpp"
#include "indri/DiskDocListIterator.hpp"

using namespace indri::index;

//
// IndexWriter constructor
//

IndexWriter::IndexWriter()
{
}

//
// _writeSkip
//

void IndexWriter::_writeSkip( SequentialWriteBuffer* buffer, int document, int length ) {
  buffer->write( &document, sizeof(int) );
  buffer->write( &length, sizeof(int) );
}

//
// _writeBatch
//

void IndexWriter::_writeBatch( SequentialWriteBuffer* buffer, int document, int length, Buffer& data ) {
  _writeSkip( buffer, document, length );
  buffer->write( data.front(), data.position() );
  data.clear();
}

//
// _writeManifest
//

void IndexWriter::_writeManifest( const std::string& path ) {
  Parameters manifest;

  manifest.set( "type", "DiskIndex" );
  manifest.set( "code-build-date", __DATE__ );

  manifest.set( "corpus", "" );
  Parameters corpus = manifest["corpus"];

  corpus.set("total-documents", _corpus.totalDocuments);
  corpus.set("total-terms", (UINT64) _corpus.totalTerms);
  corpus.set("unique-terms", _corpus.uniqueTerms);
  corpus.set("document-base", _documentBase);

  manifest.set( "fields", "" );
  Parameters fields = manifest["fields"];

  for( int i=0; i<_fields.size(); i++ ) {
    fields.append("field");

    fields[i].set("isNumeric", _fields[i].numeric);
    fields[i].set("name", _fields[i].name);
    fields[i].set("total-documents", _fieldData[i].documentCount);
    fields[i].set("total-terms", (UINT64) _fieldData[i].totalCount);
  }

  manifest.writeFile( path );
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
  std::string manifestPath = Path::combine( path, "manifest" );

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
  
  std::vector<WriterIndexContext*> contexts;
  _buildIndexContexts( contexts, indexes );
  _writeInvertedLists( contexts );
  _writeFieldLists( indexes, path );
  _writeDirectLists( contexts, _directFile );
  delete_vector_contents( contexts );

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


  // write a manifest file
  _writeManifest( manifestPath );
}

//
// _buildIndexContexts
//

void IndexWriter::_buildIndexContexts( std::vector<WriterIndexContext*>& contexts, std::vector<indri::index::Index*>& indexes ) {
  for( int i=0; i<indexes.size(); i++ )
    contexts.push_back( new WriterIndexContext( indexes[i]->docListFileIterator(), indexes[i] ) );
}

//
// _writeFieldLists
//

void IndexWriter::_writeFieldLists( std::vector<indri::index::Index*>& indexes, const std::string& path ) {
  if( indexes.size() == 0 )
    return;
  
  for( int field=1; field<=_fields.size(); field++ ) {
    std::stringstream fieldName;
    fieldName << "field" << field;
    std::string fieldPath = Path::combine( path, fieldName.str() );

    std::vector<indri::index::DocExtentListIterator*> iterators;
    for( int i=0; i<indexes.size(); i++ )
      iterators.push_back( indexes[i]->fieldListIterator( field ) ); 

    _writeFieldList( fieldPath, iterators );
  }
}

//
// _fetchMatchingInvertedLists
//

void IndexWriter::_fetchMatchingInvertedLists( greedy_vector<WriterIndexContext*>& lists, invertedlist_pqueue& queue ) {
  lists.clear();

  WriterIndexContext* first = queue.top();
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

void IndexWriter::_pushInvertedLists( greedy_vector<WriterIndexContext*>& lists, invertedlist_pqueue& queue ) {
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

void IndexWriter::_writeStatistics( greedy_vector<WriterIndexContext*>& lists, indri::index::TermData* termData ) {
  greedy_vector<WriterIndexContext*>::iterator iter;
  ::termdata_clear( termData, _fields.size() );

  // find out what term we're writing
  strcpy( const_cast<char*>(termData->term), lists[0]->iterator->currentEntry()->termData->term );

  for( iter = lists.begin(); iter != lists.end(); ++iter ) {
    indri::index::DocListFileIterator::DocListData* listData = (*iter)->iterator->currentEntry();
    ::termdata_merge( termData, listData->termData,  _fields.size() );
  }

  _termDataBuffer.clear();
  RVLCompressStream stream( _termDataBuffer );

  stream << termData->term;
  ::termdata_compress( stream, termData,  _fields.size() );

  int dataSize = stream.dataSize();
  _invertedOutput->write( &dataSize, sizeof(int) );
  _invertedOutput->write( stream.data(), stream.dataSize() );

  _corpus.uniqueTerms++;
  _corpus.totalTerms += termData->corpus.totalCount;
}

//
// _writeFieldList
//
// field list is:
//  control byte -- currently no options available here
//  document / 
//

void IndexWriter::_writeFieldList( const std::string& fileName, std::vector<indri::index::DocExtentListIterator*> iterators ) {
  File outputFile;
  outputFile.create( fileName );
  SequentialWriteBuffer output( outputFile, 1024*1024 );
  
  // write a control byte
  UINT8 control = 0;
  output.write( &control, sizeof(UINT8) );

  Buffer dataBuffer;
  const int minimumSkip = 1<<12; //4k
  int lastDocument = 0;

  for( int i=0; i<iterators.size(); i++ ) {
    DocExtentListIterator* iterator = iterators[i];

    if( !iterator )
      continue;

    iterator->startIteration();
    RVLCompressStream stream( dataBuffer );

    while( iterator->currentEntry() ) {
      DocExtentListIterator::DocumentExtentData* entry = iterator->currentEntry();

      if( dataBuffer.position() > minimumSkip ) {
        _writeBatch( &output, entry->document, dataBuffer.position(), dataBuffer );
        lastDocument = 0;
      }

      // add document difference
      stream << ( entry->document - lastDocument );
      lastDocument = entry->document;

      // extent count
      int count = entry->extents.size();
      stream << count;

      // extents and numbers
      int lastPosition = 0;
      for( int j=0; j<count; j++ ) {
        Extent& extent = entry->extents[j];

        stream << (extent.begin - lastPosition);
        lastPosition = extent.begin;
        stream << (extent.end - lastPosition);
        lastPosition = extent.end;

        if( entry->numbers.size() )
          stream << entry->numbers[j];
      }
    }
  }

  _writeBatch( &output, -1, dataBuffer.position(), dataBuffer );
  output.flush();
  outputFile.close();
}

//
// _addInvertedListData
//
// Inverted list is:
//   termData (as written by _writeStatistics)
//   control byte -- hasTopdocs(0x1)
//   optional topdocs list: topdocsCount + (document/count/length)+
//   ( [skip: document/skipLength] (doc/positionCount/positions+)+ )
// (-1) signifies there's no more skips
// 
//

void IndexWriter::_addInvertedListData( greedy_vector<WriterIndexContext*>& lists, indri::index::TermData* termData, Buffer& listBuffer, UINT64& startOffset, UINT64& endOffset ) {
  greedy_vector<WriterIndexContext*>::iterator iter;
  const int minimumSkip = 1<<12; // 4k
  int documentsWritten = 0;

  const float topdocsFraction = 0.01f;
  bool hasTopdocs = termData->corpus.documentCount > TOPDOCS_DOCUMENT_COUNT;
  bool isFrequent = termData->corpus.totalCount > FREQUENT_TERM_COUNT;
  int topdocsCount = hasTopdocs ? int(termData->corpus.totalCount * 0.01) : 0;
  int topdocsSpace = hasTopdocs ? topdocsCount*3*sizeof(DocListIterator::TopDocument) + sizeof(int) : 0;

  // compress term data
  _termDataBuffer.clear(); 
  RVLCompressStream termStream( _termDataBuffer );

  termStream << termData->term;
  ::termdata_compress( termStream, termData, _fieldData.size() );

  // write term data
  startOffset = _invertedOutput->tell();

  int termDataLength = termStream.dataSize();
  _invertedOutput->write( &termDataLength, sizeof termDataLength );
  _invertedOutput->write( termStream.data(), termStream.dataSize() );

  // write a control byte
  char control = (hasTopdocs ? 0x01 : 0) | (isFrequent ? 0x02 : 0);
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
        _writeBatch( _invertedOutput, documentData->document, listBuffer.position(), listBuffer );

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
  _writeBatch( _invertedOutput, -1, listBuffer.position(), listBuffer );
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

  endOffset = finalPosition;
}

//
// _storeTermEntry
//

void IndexWriter::_storeTermEntry( IndexWriter::keyfile_pair& pair, indri::index::TermData* termData, INT64 startOffset, INT64 endOffset, int termID ) {
  DiskTermData diskData;

  diskData.length = endOffset - startOffset;
  diskData.startOffset = startOffset;
  diskData.termData = termData;
  diskData.termID = termID;

  // add term data to id map (storing term string)
  _termDataBuffer.clear();
  RVLCompressStream idStream( _termDataBuffer );

  disktermdata_compress( idStream, &diskData, _fieldData.size(), indri::index::DiskTermData::WithString |
                                                                 indri::index::DiskTermData::WithOffsets );

  pair.idMap->put( termID, idStream.data(), idStream.dataSize() );

  // add term data to string map (storing termID)
  _termDataBuffer.clear();
  RVLCompressStream stringStream( _termDataBuffer );

  disktermdata_compress( stringStream, &diskData, _fieldData.size(), indri::index::DiskTermData::WithTermID |
                                                                     indri::index::DiskTermData::WithOffsets );

  pair.stringMap->put( termData->term, stringStream.data(), stringStream.dataSize() );
}

//
// _storeFrequentTerms
//

void IndexWriter::_storeFrequentTerms( const std::string& fileName ) {
  // sort the _topTerms vector by term count
  std::sort( _topTerms.begin(), _topTerms.end(), top_term_entry::greater() );

  for( int i=0; i<_topTerms.size(); i++ ) {
    int termID = i+1;
    _storeTermEntry( _frequentTerms, _topTerms[i].termData, _topTerms[i].startOffset, _topTerms[i].endOffset, termID );
  }

  // store data in a file, too
  File output;
  output.open( fileName );
  SequentialWriteBuffer writeBuffer( output, 1024*1024 );
  Buffer intermediateBuffer( 16*1024 );
  RVLCompressStream stream( intermediateBuffer );

  for( int i=0; i<_topTerms.size(); i++ ) { 
    indri::index::DiskTermData diskTermData;

    diskTermData.startOffset = _topTerms[i].startOffset;
    diskTermData.length = _topTerms[i].endOffset - _topTerms[i].startOffset;
    diskTermData.termData = _topTerms[i].termData;
    diskTermData.termID = i+1;

    ::disktermdata_compress( stream,
                             &diskTermData,
                             _fields.size(),
                             indri::index::DiskTermData::WithOffsets |
                             indri::index::DiskTermData::WithString |
                             indri::index::DiskTermData::WithTermID );

    writeBuffer.write( intermediateBuffer.front(), intermediateBuffer.size() );
    intermediateBuffer.clear();
  }

  writeBuffer.flush();
  output.close();
}

//
// _storeMatchInformation
//

void IndexWriter::_storeMatchInformation( greedy_vector<WriterIndexContext*>& lists, int sequence, indri::index::TermData* termData, UINT64 startOffset, UINT64 endOffset ) {
  bool isFrequent = termData->corpus.documentCount > FREQUENT_TERM_COUNT;

  for( int i=0; i<lists.size(); i++ ) {
    WriterIndexContext* list = lists[i];
    indri::index::DiskDocListIterator* iterator = dynamic_cast<DiskDocListIterator*>(lists[i]->iterator);
    bool wasFrequent = (iterator && iterator->isFrequent());

    if( !wasFrequent ) {
      if( !isFrequent ) {
        // common case--remaining infrequent
        list->bitmap->add( list->infrequentIndex, sequence );
      } else if( isFrequent ) {
        // becoming frequent
        list->newlyFrequent->add( list->infrequentIndex, termData->term );
      }
    } else {
      if( !isFrequent ) {
        // want to grab the sequence now so it's easy to find what this term maps to
        list->newlyInfrequent->add( sequence, termData->term );
      } else {
        // do nothing
      }
    }

    if( !isFrequent )
      list->infrequentIndex++;
  }

  if( isFrequent ) {
    top_term_entry entry;

    entry.termData = ::termdata_create(  _fields.size() );
    ::termdata_merge( entry.termData, termData,  _fields.size() );
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

void IndexWriter::_writeInvertedLists( std::vector<WriterIndexContext*>& contexts ) {
  
  // write a combined inverted list in vocabulary order
  // in the process, create a new list of termIDs from the old list
  
  std::priority_queue<WriterIndexContext*,
                      std::vector<WriterIndexContext*>,
                      WriterIndexContext::less> invertedLists;
  Buffer invertedListBuffer;

  UINT64 startOffset;
  UINT64 endOffset;

  // clear out the term buffer
  char term[Keyfile::MAX_KEY_LENGTH+1];
  term[0] = 0;
  int sequence = 1;

  _documentBase = contexts[0]->index->documentBase();

  for( int i=0; i<contexts.size(); i++ ) {
    invertedLists.push( contexts[i] );
    _corpus.totalDocuments += contexts[i]->index->documentCount();
  }

  greedy_vector<WriterIndexContext*> current;
  indri::index::TermData* termData = ::termdata_create( _fields.size() );
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
    _storeMatchInformation( current, sequence, termData, startOffset, endOffset );

    // push back all doc lists with useful information
    _pushInvertedLists( current, invertedLists );

    _corpus.uniqueTerms++;
  }

  // at this point, we need to fill in all the "top" vocabulary data
  // into the keyfile

  ::termdata_delete( termData, _fields.size() );
  _invertedOutput->flush();
  delete _invertedOutput;
  _invertedFile.close();
}

//
// _lookupTermID
//

int IndexWriter::_lookupTermID( Keyfile& keyfile, const char* term ) {
  char compressedData[16*1024];
  char uncompressedData[16*1024];
  int actual;

  bool result = keyfile.get( term, compressedData, actual, sizeof compressedData );
  
  if( !result )
    return 0;

  RVLDecompressStream stream( compressedData, actual );
  DiskTermData* diskTermData = ::disktermdata_decompress( stream,
                                                          uncompressedData,
                                                          _fields.size(),
                                                          DiskTermData::WithOffsets | DiskTermData::WithTermID );

  return diskTermData->termID;
}

//
// _harvestTerms
//

indri::index::TermTranslator* IndexWriter::_buildTermTranslator( Keyfile& newInfrequentTerms,
                                            Keyfile& newFrequentTerms,
                                            TermRecorder& oldFrequentTermsRecorder,
                                            TermRecorder& newFrequentTermsRecorder,
                                            TermRecorder& newlyInfrequentTermsRecorder,
                                            TermBitmap* bitmap )
{
  // already have the bitmap, the recorder.  Want to make a translator

  // 1. map frequent terms to frequent terms
  std::vector<int>* frequent = new std::vector<int>;

  std::vector< std::pair<const char*, int> > missing;
  oldFrequentTermsRecorder.sort();
  oldFrequentTermsRecorder.buildMap( *frequent, newFrequentTermsRecorder, &missing );

  // 2. map frequent terms to infrequent terms
  for( int i=0; i<missing.size(); i++ ) {
    int missingTerm = missing[i].second;
    (*frequent)[missingTerm] = _lookupTermID( newInfrequentTerms, missing[i].first );
  }

  // 3. map infrequent terms to frequent terms
  HashTable<int, int>* wasInfrequentMap = new HashTable<int, int>();
  newlyInfrequentTermsRecorder.sort();
  std::vector< std::pair< const char*, int > >& newlyInfrequentPairs = newlyInfrequentTermsRecorder.pairs();

  for( int i=0; i<newlyInfrequentPairs.size(); i++ ) {
    // lookup newlyInfrequentTerms[i]
    int oldTermID = newlyInfrequentPairs[i].second;
    int newTermID = _lookupTermID( newFrequentTerms, newlyInfrequentPairs[i].first );
    wasInfrequentMap->insert( oldTermID, newTermID );
  }

  // 4. infrequent to infrequent is easy--bitmap takes care of it
  //    just get the appropriate counts from the maps
  // new frequent count = previouslyFrequent - becameInfrequent + becameFrequent

  int oldFrequentCount = frequent->size();
  int newFrequentCount = frequent->size() - missing.size() + wasInfrequentMap->size();

  return new TermTranslator( oldFrequentCount,
                             newFrequentCount,
                             frequent,
                             wasInfrequentMap,
                             bitmap );
}

//
// _writeDirectLists
//

void IndexWriter::_writeDirectLists( WriterIndexContext* context, SequentialWriteBuffer* output ) {
  // have to grab a list of all the old frequent terms first--how is this done?
  VocabularyIterator* vocabulary = context->index->frequentVocabularyIterator();
  indri::index::Index* index = context->index;
  
  while( !vocabulary->finished() ) {
    indri::index::DiskTermData* diskTermData = vocabulary->currentEntry();

    context->oldFrequent->add( diskTermData->termID, diskTermData->termData->term );
    vocabulary->nextEntry();
  }


  TermListFileIterator* iterator = index->termListFileIterator();
  TermTranslator* translator = _buildTermTranslator( *_infrequentTerms.idMap,
                                                     *_frequentTerms.idMap,
                                                     *context->oldFrequent,
                                                     *context->newlyFrequent,
                                                     *context->newlyInfrequent,
                                                     context->bitmap );
  iterator->startIteration();
  TermList writeList;
  Buffer outputBuffer( 256*1024 );

  while( !iterator->finished() ) {
    writeList.clear();
    TermList* list = iterator->currentEntry();

    // copy and translate terms
    for( int i=0; i<list->terms().size(); i++ ) {
      int currentTerm = list->terms()[i];
      int translated = (*translator)( currentTerm );

      writeList.addTerm( translated );
    }

    // copy field data
    for( int i=0; i<list->fields().size(); i++ ) {
      writeList.addField( list->fields()[i] );
    }
  
    writeList.write( outputBuffer );

    if( outputBuffer.position() > 128*1024 ) {
      output->write( outputBuffer.front(), outputBuffer.size() );
      outputBuffer.clear();
    }
  }

  output->write( outputBuffer.front(), outputBuffer.size() );
  outputBuffer.clear();
}

//
// _writeDirectLists
//

void IndexWriter::_writeDirectLists( std::vector<WriterIndexContext*>& contexts, File& directFile ) {
  std::vector<WriterIndexContext*>::iterator iter;
  SequentialWriteBuffer* buffer = new SequentialWriteBuffer( directFile, 1024*1024 );

  for( iter = contexts.begin(); iter != contexts.end(); iter++ ) {
    _writeDirectLists( *iter, buffer );
  }
}

