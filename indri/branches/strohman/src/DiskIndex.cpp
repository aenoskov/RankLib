
//
// DiskIndex
//
// 8 December 2004 -- tds
//

#include "indri/DiskIndex.hpp"
#include "lemur/Keyfile.hpp"
#include "indri/DiskDocListIterator.hpp"
#include "indri/DiskDocExtentListIterator.hpp"
#include "indri/DiskDocListFileIterator.hpp"

//
// _fetchTermData
//

indri::index::DiskTermData* indri::index::DiskIndex::_fetchTermData( int termID ) {
  char buffer[16*1024];
  int actual;

  bool result = _frequentStringToTerm.get( termID, buffer, actual, sizeof buffer );

  if( !result ) {
    result = _infrequentStringToTerm.get( termID, buffer, actual, sizeof buffer );

    if( !result )
      return 0;
  }
  assert( result );

  RVLDecompressStream stream( buffer, actual );
  return disktermdata_decompress( stream, _fieldData.size(), DiskTermData::WithTermID | DiskTermData::WithOffsets );
}

//
// _fetchTermData
//

indri::index::DiskTermData* indri::index::DiskIndex::_fetchTermData( const char* term ) {
  char buffer[16*1024];
  int actual;

  bool result = _frequentStringToTerm.get( term, buffer, actual, sizeof buffer );

  if( !result ) {
    result = _infrequentStringToTerm.get( term, buffer, actual, sizeof buffer );

    if( !result )
      return 0;
  }
  assert( result );
  RVLDecompressStream stream( buffer, actual );

  return disktermdata_decompress( stream, _fieldData.size(), DiskTermData::WithString | DiskTermData::WithOffsets );
}

//
// documentBase
//

int indri::index::DiskIndex::documentBase() {
  return _documentBase;
}

//
// term
//

int indri::index::DiskIndex::term( const char* t ) {
  indri::index::DiskTermData* diskTermData = _fetchTermData( t );
  int termID = diskTermData->termID;
  ::disktermdata_delete( diskTermData );
  return termID;
}

//
// term
//

int indri::index::DiskIndex::term( const std::string& t ) {
  return term( t.c_str() );
}

//
// term
//

std::string indri::index::DiskIndex::term( int termID ) {
  std::string result;
  indri::index::DiskTermData* diskTermData = _fetchTermData( termID );
  result = diskTermData->termData->term;
  ::disktermdata_delete( diskTermData );

  return result;
}

//
// documentLength
//

int indri::index::DiskIndex::documentLength( int documentID ) {
  int documentOffset = documentID - _documentBase;

  if( documentOffset < 0 || _corpusStatistics.totalDocuments >= documentOffset ) 
    return 0;

  int length;
  UINT64 offset = sizeof(length) + documentOffset;

  _documentLengths.read( &length, offset, sizeof(length) );
  return length;
}

//
// documentCount
//

UINT64 indri::index::DiskIndex::documentCount() {
  return _corpusStatistics.totalDocuments;
}

//
// termCount
//

UINT64 indri::index::DiskIndex::termCount() {
  return _corpusStatistics.totalTerms;
}

//
// uniqueTermCount
//

UINT64 indri::index::DiskIndex::uniqueTermCount() {
  return _corpusStatistics.uniqueTerms;
}

//
// termCount
//

UINT64 indri::index::DiskIndex::termCount( const std::string& t ) {
  DiskTermData* diskTermData = _fetchTermData( t.c_str() );
  UINT64 count = diskTermData->termData->corpus.totalCount;
  ::disktermdata_delete( diskTermData );
  return count;
}

//
// fieldTermCount
//

UINT64 indri::index::DiskIndex::fieldTermCount( const std::string& f, const std::string& t ) {
  DiskTermData* diskTermData = _fetchTermData( t.c_str() );
  int index = field( f );
  UINT64 count = diskTermData->termData->fields[index].totalCount;
  ::disktermdata_delete( diskTermData );
  return count;
}

//
// fieldTermCount
//

UINT64 indri::index::DiskIndex::fieldTermCount( const std::string& f ) {
  int index = field( f );
  UINT64 count = _fieldData[index].totalCount;
  return count;
}

//
// fieldDocumentCount
//

UINT64 indri::index::DiskIndex::fieldDocumentCount( const std::string& f ) {
  int index = field( f );
  UINT64 count = _fieldData[index].documentCount;
  return count;
}

//
// fieldDocumentCount
//

UINT64 indri::index::DiskIndex::fieldDocumentCount( const std::string& f, const std::string& t ) {
  DiskTermData* diskTermData = _fetchTermData( t.c_str() );
  int index = field( f );
  UINT64 count = diskTermData->termData->fields[index].documentCount;
  ::disktermdata_delete( diskTermData );
  return count;
}

//
// docListIterator
//

indri::index::DocListIterator* indri::index::DiskIndex::docListIterator( int termID ) {
  // find out where the iterator starts and ends
  DiskTermData* data = _fetchTermData( termID );

  // if no such term, quit
  if( !data )
    return 0;

  INT64 startOffset = data->startOffset;
  INT64 length = data->length;
  ::disktermdata_delete( data );

  // truncate the length argument at 1MB, use it to pick a size for the readbuffer
  length = lemur_compat::min<INT64>( length, 1024*1024 );

  return new DiskDocListIterator( new SequentialReadBuffer( _invertedFile, length ), startOffset );
}

//
// docListIterator 
//

indri::index::DocListIterator* indri::index::DiskIndex::docListIterator( const std::string& term ) {
  // find out where the iterator starts and ends
  DiskTermData* data = _fetchTermData( term.c_str() );

  // if no such term, quit
  if( !data )
    return 0;

  INT64 startOffset = data->startOffset;
  INT64 length = data->length;
  ::disktermdata_delete( data );

  // truncate the length argument at 1MB, use it to pick a size for the readbuffer
  length = lemur_compat::min<INT64>( length, 1024*1024 );

  return new DiskDocListIterator( new SequentialReadBuffer( _invertedFile, length ), startOffset );
}

//
// docListFileIterator
//

indri::index::DocListFileIterator* indri::index::DiskIndex::docListFileIterator( ) {
  return new DiskDocListFileIterator( _invertedFile, _fieldData.size() );
}

//
// fieldListIterator
//

indri::index::DocExtentListIterator* indri::index::DiskIndex::fieldListIterator( int fieldID ) {
  if( fieldID == 0 || fieldID > _fieldData.size() ) {
    return 0;
  }

  File& fieldFile = _fieldFiles[fieldID-1];
  return new DiskDocExtentListIterator( new SequentialReadBuffer( fieldFile ), 0 );
}

//
// fieldListIterator
//

indri::index::DocExtentListIterator* indri::index::DiskIndex::fieldListIterator( const std::string& fieldName ) {
  int fieldID = field( fieldName );
  
  if( fieldID == 0 )
    return 0;

  File& fieldFile = _fieldFiles[fieldID-1];
  return new DiskDocExtentListIterator( new SequentialReadBuffer( fieldFile ), 0 );
}

//
// termListFileIterator
//

const indri::index::TermList* indri::index::DiskIndex::termList( int documentID ) {
  // TODO: return appropriate data
  assert( 0 && "unimplemented" );
  return 0;
}

//
// termListFileIterator
//

indri::index::TermListFileIterator* indri::index::DiskIndex::termListFileIterator() {
  // TODO: return appropriate data
  assert( 0 && "unimplemented" );
  return 0;
}

//
// vocabularyIterator
//

indri::index::VocabularyIterator* indri::index::DiskIndex::vocabularyIterator() {
  // TODO: return appropriate data
  assert( 0 && "unimplemented" );
  return 0;
}

