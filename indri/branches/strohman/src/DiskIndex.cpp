
//
// DiskIndex
//
// 8 December 2004 -- tds
//

#include "indri/DiskIndex.hpp"
#include "lemur/Keyfile.hpp"

//
// _fetchTermData
//

indri::index::TermData* indri::index::DiskIndex::_fetchTermData( int termID ) {
  char buffer[16*1024];
  int actual;

  bool result = _frequentStringToTerm.get( termID, buffer, actual, sizeof buffer );

  if( !result ) {
    result = _infrequentStringToTerm.get( termID, buffer, actual, sizeof buffer );

    if( !result )
      return 0;
  }
  assert( result );

  assert( 0 && "this code doesn't actually work: look at IndexWriter to make sure they match" );
  RVLDecompressStream stream( buffer, actual );
  TermData* termData = ::termdata_create( _fieldData.size() );
  ::termdata_decompress( stream, termData, _fieldData.size() );

  return termData;
}

//
// _fetchTermData
//

indri::index::TermData* indri::index::DiskIndex::_fetchTermData( const char* term ) {
  char buffer[16*1024];
  int actual;

  bool result = _frequentStringToTerm.get( term, buffer, actual, sizeof buffer );

  if( !result ) {
    result = _infrequentStringToTerm.get( term, buffer, actual, sizeof buffer );

    if( !result )
      return 0;
  }
  assert( result );

  assert( 0 && "this code doesn't actually work: look at IndexWriter to make sure they match" );
  RVLDecompressStream stream( buffer, actual );
  TermData* termData = ::termdata_create( _fieldData.size() );
  ::termdata_decompress( stream, termData, _fieldData.size() );

  return termData;
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
  indri::index::TermData* termData = _fetchTermData( t );
  int termID = 0; // TODO: fix me termData->termID;
  ::termdata_delete( termData, _fieldData.size() );
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
  indri::index::TermData* termData = _fetchTermData( termID );
  result = termData->term;
  ::termdata_delete( termData, _fieldData.size() );

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
  indri::index::TermData* termData = _fetchTermData( t.c_str() );
  UINT64 count = termData->corpus.totalCount;
  ::termdata_delete( termData, _fieldData.size() );
  return count;
}

//
// fieldTermCount
//

UINT64 indri::index::DiskIndex::fieldTermCount( const std::string& f, const std::string& t ) {
  indri::index::TermData* termData = _fetchTermData( t.c_str() );
  int index = field( f );
  UINT64 count = termData->fields[index].totalCount;
  ::termdata_delete( termData, _fieldData.size() );
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
  indri::index::TermData* termData = _fetchTermData( t.c_str() );
  int index = field( f );
  UINT64 count = termData->fields[index].documentCount;
  ::termdata_delete( termData, _fieldData.size() );
  return count;
}

//
// docListIterator
//

indri::index::DocListIterator* indri::index::DiskIndex::docListIterator( int termID ) {
  // look up termID in Keyfile
  // find offset to inverted list information
  // make a DocListIterator object that knows the start and end offsets of the data
  // return it

  // TODO: return appropriate data
  assert( 0 && "unimplemented" );
  return 0;
}

//
// docListIterator 
//

indri::index::DocListIterator* indri::index::DiskIndex::docListIterator( const std::string& term ) {
  // look up termID in Keyfile
  // find offset to inverted list information
  // make a DocListIterator object that knows the start and end offsets of the data
  // return it

  // TODO: return appropriate data
  assert( 0 && "unimplemented" );
  return 0;
}

//
// docListFileIterator
//

indri::index::DocListIterator* indri::index::DiskIndex::docListFileIterator( ) {
  // TODO: return appropriate data
  assert( 0 && "unimplemented" );
  return 0;
}

//
// fieldListIterator
//

indri::index::DocExtentListIterator* indri::index::fieldListIterator( int fieldID ) {
  // TODO: return appropriate data
  assert( 0 && "unimplemented" );
  return 0;
}

//
// fieldListIterator
//

indri::index::DocExtentListIterator* indri::index::fieldListIterator( const std::string& field ) {
  // TODO: return appropriate data
  assert( 0 && "unimplemented" );
  return 0;
}

//
// termListFileIterator
//

const indri::index::TermList* termList( int documentID ) {
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

