
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
#include "indri/DocumentDataIterator.hpp"

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
  manifest.set( "indri-distribution", INDRI_DISTRIBUTION );

  manifest.set( "corpus", "" );
  Parameters corpus = manifest["corpus"];

  corpus.set("total-documents", _corpus.totalDocuments);
  corpus.set("total-terms", (UINT64) _corpus.totalTerms);
  corpus.set("unique-terms", _corpus.uniqueTerms);
  corpus.set("document-base", _documentBase);
  corpus.set("frequent-terms", (int)_topTerms.size());

  manifest.set( "fields", "" );
  Parameters fields = manifest["fields"];

  for( int i=0; i<_fields.size(); i++ ) {
    fields.append("field");
    Parameters field = fields["field"];

    field[i].set("isNumeric", _fields[i].numeric);
    field[i].set("name", _fields[i].name);
    field[i].set("total-documents", _fieldData[i].documentCount);
    field[i].set("total-terms", (UINT64) _fieldData[i].totalCount);
  }

  manifest.writeFile( path );
}

//
// write
//

void IndexWriter::write( indri::index::Index& index, std::vector<indri::index::Index::FieldDescription>& fields, const std::string& path ) {
  std::vector< indri::index::Index* > indexes;
  indexes.push_back( &index );
  write( indexes, fields, path );
}

//
// write
//

void IndexWriter::write( std::vector<Index*>& indexes, std::vector<indri::index::Index::FieldDescription>& fields, const std::string& path ) {
  Path::create( path );
  _fields = fields;

  std::string frequentStringPath = Path::combine( path, "frequentString" );
  std::string infrequentStringPath = Path::combine( path, "infrequentString" );
  std::string frequentIDPath = Path::combine( path, "frequentID" );
  std::string infrequentIDPath = Path::combine( path, "infrequentID" );
  std::string frequentTermsDataPath = Path::combine( path, "frequentTerms" );
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
  _frequentTermsData.create( frequentTermsDataPath );

  // stats, inverted file, direct file
  _documentStatistics.create( documentStatisticsPath );
  _documentLengths.create( documentLengthsPath );
  _invertedFile.create( invertedFilePath );
  _directFile.create( directFilePath );

  _invertedOutput = new SequentialWriteBuffer( _invertedFile, 1024*1024 );

  std::vector<WriterIndexContext*> contexts;
  _buildIndexContexts( contexts, indexes );

  _writeInvertedLists( contexts );
  _writeFieldLists( indexes, path );
  _writeDirectLists( contexts );
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
  _frequentTermsData.close();

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
    contexts.push_back( new WriterIndexContext( indexes[i] ) );
}

//
// _writeFieldLists
//

void IndexWriter::_writeFieldLists( std::vector<indri::index::Index*>& indexes, const std::string& path ) {
  if( indexes.size() == 0 )
    return;
  
  for( int field=0; field<_fields.size(); field++ ) {
    std::stringstream fieldName;
    int fieldID = field+1;
    fieldName << "field" << fieldID;
    std::string fieldPath = Path::combine( path, fieldName.str() );

    std::vector<indri::index::DocExtentListIterator*> iterators;
    for( int i=0; i<indexes.size(); i++ )
      iterators.push_back( indexes[i]->fieldListIterator( field+1 ) ); 

    _fieldData.push_back( FieldStatistics( _fields[field].name, _fields[field].numeric, 0, 0 ) );
    _writeFieldList( fieldPath, field, iterators );
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

    if( !lists[i]->iterator->finished() )
      queue.push( lists[i] );
  }
}

//
// _writeStatistics
//

void IndexWriter::_writeStatistics( greedy_vector<WriterIndexContext*>& lists, indri::index::TermData* termData, UINT64& startOffset ) {
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

  startOffset = _invertedOutput->tell();

  int dataSize = stream.dataSize();
  _invertedOutput->write( &dataSize, sizeof(UINT32) );
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

void IndexWriter::_writeFieldList( const std::string& fileName, int fieldIndex, std::vector<indri::index::DocExtentListIterator*> iterators ) {
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

      assert( entry->document > lastDocument || lastDocument == 0 );
      int terms = 0;

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

        assert( extent.begin - lastPosition >= 0 );
        assert( extent.end - extent.begin >= 0 );

        stream << (extent.begin - lastPosition);
        lastPosition = extent.begin;
        stream << (extent.end - lastPosition);
        lastPosition = extent.end;
        terms += (extent.end - extent.begin);

        if( entry->numbers.size() )
          stream << entry->numbers[j];
      }

      assert( _fieldData.size() > fieldIndex );
      _fieldData[fieldIndex].documentCount++;
      _fieldData[fieldIndex].totalCount += terms;

      iterator->nextEntry();
    }

    delete iterator;
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

void IndexWriter::_addInvertedListData( greedy_vector<WriterIndexContext*>& lists, indri::index::TermData* termData, Buffer& listBuffer, UINT64& endOffset ) {
  greedy_vector<WriterIndexContext*>::iterator iter;
  const int minimumSkip = 1<<12; // 4k
  int documentsWritten = 0;

  const float topdocsFraction = 0.01f;
  bool hasTopdocs = termData->corpus.documentCount > TOPDOCS_DOCUMENT_COUNT;
  bool isFrequent = termData->corpus.totalCount > FREQUENT_TERM_COUNT;
  int topdocsCount = hasTopdocs ? int(termData->corpus.totalCount * 0.01) : 0;
  int topdocsSpace = hasTopdocs ? topdocsCount*3*sizeof(int) + sizeof(int) : 0;

  // write a control byte
  char control = (hasTopdocs ? 0x01 : 0) | (isFrequent ? 0x02 : 0);
  _invertedOutput->write( &control, 1 );

  UINT64 initialPosition = _invertedOutput->tell();

  // leave some room for the topdocs list
  if( hasTopdocs ) {
    _invertedOutput->write( &topdocsCount, sizeof(int) );
    _invertedOutput->seek( topdocsSpace + initialPosition );
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
      if( hasTopdocs ) {
        topdocs.push( topDocument );
        while( topdocs.size() > topdocsCount )
          topdocs.pop();
      }

      if( listBuffer.position() > minimumSkip ) {
        // time to write in a skip
        _writeBatch( _invertedOutput, documentData->document, listBuffer.position(), listBuffer );

        // delta encode documents by batch
        lastDocument = 0;
      }

      assert( documentData->document > lastDocument );

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
      DocListIterator::TopDocument topDocument = topdocs.top();
      
      _invertedOutput->write( &topDocument.document, sizeof(int) );
      _invertedOutput->write( &topDocument.count, sizeof(int) );
      _invertedOutput->write( &topDocument.length, sizeof(int) );
      topdocs.pop();
    }

    assert( (_invertedOutput->tell() - initialPosition) == topdocsSpace );
    _invertedOutput->seek( finalPosition );
  }

  endOffset = finalPosition;
}

//
// _storeTermEntry
//

void IndexWriter::_storeTermEntry( IndexWriter::keyfile_pair& pair, indri::index::DiskTermData* diskTermData ) {
  // add term data to id map (storing term string)
  _termDataBuffer.clear();
  RVLCompressStream idStream( _termDataBuffer );

  disktermdata_compress( idStream, diskTermData, _fields.size(), indri::index::DiskTermData::WithString |
                                                                    indri::index::DiskTermData::WithOffsets );

  pair.idMap->put( diskTermData->termID, idStream.data(), idStream.dataSize() );

  // add term data to string map (storing termID)
  _termDataBuffer.clear();
  RVLCompressStream stringStream( _termDataBuffer );

  disktermdata_compress( stringStream, diskTermData, _fields.size(), indri::index::DiskTermData::WithTermID |
                                                                        indri::index::DiskTermData::WithOffsets );

  pair.stringMap->put( diskTermData->termData->term, stringStream.data(), stringStream.dataSize() );
}

//
// _storeFrequentTerms
//

void IndexWriter::_storeFrequentTerms() {
  // sort the _topTerms vector by term count
  std::sort( _topTerms.begin(), _topTerms.end(), disktermdata_greater() );

  for( int i=0; i<_topTerms.size(); i++ ) {
    int termID = i+1;
    _topTerms[i]->termID = termID;
    _storeTermEntry( _frequentTerms, _topTerms[i] );
  }

  // store data in a file, too
  SequentialWriteBuffer writeBuffer( _frequentTermsData, 1024*1024 );
  Buffer intermediateBuffer( 16*1024 );
  RVLCompressStream stream( intermediateBuffer );

  for( int i=0; i<_topTerms.size(); i++ ) { 
    _topTerms[i]->termID = i+1;

    ::disktermdata_compress( stream,
                             _topTerms[i],
                             _fields.size(),
                             indri::index::DiskTermData::WithOffsets |
                             indri::index::DiskTermData::WithString |
                             indri::index::DiskTermData::WithTermID );

    writeBuffer.write( intermediateBuffer.front(), intermediateBuffer.size() );
    intermediateBuffer.clear();
  }

  writeBuffer.flush();
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
    indri::index::DiskTermData* diskTermData = disktermdata_create( _fields.size() );
    ::termdata_merge( diskTermData->termData, termData, _fields.size() );
    diskTermData->startOffset = startOffset;
    diskTermData->length = endOffset - startOffset;
    strcpy( const_cast<char*>(diskTermData->termData->term), termData->term );

    _topTerms.push_back( diskTermData );
  } else {
    indri::index::DiskTermData diskTermData;

    diskTermData.termData = termData;
    diskTermData.startOffset = startOffset;
    diskTermData.length = endOffset - startOffset;
    diskTermData.termID = sequence - _topTerms.size();

    _storeTermEntry( _infrequentTerms, &diskTermData );
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
    if( !contexts[i]->iterator->finished() )
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
    _writeStatistics( current, termData, startOffset );

    // go through lists one by one, adding data to the final invlist, adding skips, etc.
    _addInvertedListData( current, termData, invertedListBuffer, endOffset );

    // have to store the termData in a B-Tree (or something) for fast access later
    _storeMatchInformation( current, sequence, termData, startOffset, endOffset );

    // push back all doc lists with useful information
    _pushInvertedLists( current, invertedLists );

    _corpus.uniqueTerms++;
  }

  // at this point, we need to fill in all the "top" vocabulary data into the keyfile
  _storeFrequentTerms();

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
// _buildTermTranslator
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

void IndexWriter::_writeDirectLists( WriterIndexContext* context,
                                    SequentialWriteBuffer* directOutput,
                                    SequentialWriteBuffer* lengthsOutput,
                                    SequentialWriteBuffer* dataOutput ) {
  // have to grab a list of all the old frequent terms first--how is this done?
  VocabularyIterator* vocabulary = context->index->frequentVocabularyIterator();
  indri::index::Index* index = context->index;
  
  vocabulary->startIteration();

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

  indri::index::DocumentDataIterator* dataIterator = context->index->documentDataIterator();
  dataIterator->startIteration();

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
  
    // record the start position
    int writeStart = outputBuffer.position();
    UINT32 length = 0;

    // write the list, leaving room for a length count
    outputBuffer.write( sizeof(UINT32) );
    writeList.write( outputBuffer );

    // record the end position, compute length
    int writeEnd = outputBuffer.position();
    length = writeEnd - (writeStart + sizeof(UINT32));

    // store length
    memcpy( outputBuffer.front() + writeStart, &length, sizeof(UINT32) );

    if( outputBuffer.position() > 128*1024 ) {
      directOutput->write( outputBuffer.front(), outputBuffer.size() );
      outputBuffer.clear();
    }

    // get a copy of the document data
    indri::index::DocumentData documentData = *dataIterator->currentEntry();

    // store offset information
    documentData.byteLength = length;
    documentData.offset = directOutput->tell() + writeStart + sizeof(UINT32);

    dataOutput->write( &documentData, sizeof(DocumentData) );
    UINT32 termLength = documentData.indexedLength;
    lengthsOutput->write( &termLength, sizeof(UINT32) );
    
    iterator->nextEntry();
    dataIterator->nextEntry();
  }

  delete iterator;
  delete dataIterator;
  directOutput->write( outputBuffer.front(), outputBuffer.size() );
  directOutput->flush();
  outputBuffer.clear();
}

//
// _writeDirectLists
//

void IndexWriter::_writeDirectLists( std::vector<WriterIndexContext*>& contexts ) {
  std::vector<WriterIndexContext*>::iterator iter;
  SequentialWriteBuffer* outputBuffer = new SequentialWriteBuffer( _directFile, 1024*1024 );
  SequentialWriteBuffer* lengthsBuffer = new SequentialWriteBuffer( _documentLengths, 1024*1024 );
  SequentialWriteBuffer* dataBuffer = new SequentialWriteBuffer( _documentStatistics, 1024*1024 );

  for( iter = contexts.begin(); iter != contexts.end(); iter++ ) {
    _writeDirectLists( *iter, outputBuffer, lengthsBuffer, dataBuffer );
  }

  outputBuffer->flush();
  lengthsBuffer->flush();
  dataBuffer->flush();

  delete outputBuffer;
  delete lengthsBuffer;
  delete dataBuffer;
}

