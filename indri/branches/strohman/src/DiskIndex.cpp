
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
#include "indri/Path.hpp"
#include "indri/Parameters.hpp"

void indri::index::DiskIndex::_readManifest( const std::string& path ) {
  Parameters manifest;
  manifest.loadFile( path );

  Parameters corpus = manifest["corpus"];

  _corpusStatistics.totalDocuments = (int) corpus["total-documents"];
  _corpusStatistics.totalTerms = (INT64) corpus["total-terms"];
  _corpusStatistics.uniqueTerms = (int) corpus["unique-terms"];
  _documentBase = (int) corpus["document-base"];

  if( manifest.exists("fields") ) {
    Parameters fields = manifest["fields"];

    for( int i=0; i<fields.size(); i++ ) {
      bool numeric = fields[i].get( "isNumeric", false );
      int documentCount = fields[i].get("total-documents", 0 );
      INT64 totalCount = fields[i].get("total-terms", INT64(0) );
      std::string name = fields[i].get( "name", "" );

      _fieldData.push_back( FieldStatistics( name, numeric, totalCount, documentCount ) );
    }
  }
}

//
// open
//

void indri::index::DiskIndex::open( const std::string& base, const std::string& relative ) {
  _path = relative;

  std::string path = Path::combine( base, relative );

  std::string frequentStringPath = Path::combine( path, "frequentString" );
  std::string infrequentStringPath = Path::combine( path, "infrequentString" );
  std::string frequentIDPath = Path::combine( path, "frequentID" );
  std::string infrequentIDPath = Path::combine( path, "infrequentID" );
  std::string documentLengthsPath = Path::combine( path, "documentLengths" );
  std::string documentStatisticsPath = Path::combine( path, "documentStatistics" );
  std::string invertedFilePath = Path::combine( path, "invertedFile" );
  std::string directFilePath = Path::combine( path, "directFile" );
  std::string manifestPath = Path::combine( path, "manifest" );

  _readManifest( manifestPath );

  _frequentStringToTerm.openRead( frequentStringPath );
  _infrequentStringToTerm.openRead( infrequentStringPath );

  _frequentIdToTerm.openRead( frequentIDPath );
  _infrequentIdToTerm.openRead( infrequentIDPath );

  _documentLengths.openRead( documentLengthsPath );
  _documentStatistics.openRead( documentStatisticsPath );

  _invertedFile.openRead( invertedFilePath );
  _directFile.openRead( directFilePath );
}

//
// close
//

void indri::index::DiskIndex::close() {
  _frequentStringToTerm.close();
  _infrequentStringToTerm.close();

  _frequentIdToTerm.close();
  _infrequentIdToTerm.close();

  _documentLengths.close();
  _documentStatistics.close();

  _invertedFile.close();
  _directFile.close();
}

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

  return disktermdata_decompress( stream, _fieldData.size(), DiskTermData::WithTermID | DiskTermData::WithOffsets );
}

//
// path
//

const std::string& indri::index::DiskIndex::path() {
  return _path;
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
// documentCount
//

UINT64 indri::index::DiskIndex::documentCount( const std::string& term ) {
  indri::index::DiskTermData* diskTermData = _fetchTermData( term.c_str() );
  UINT64 count = diskTermData->termData->corpus.documentCount;
  ::disktermdata_delete( diskTermData );
  return count;
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
// field
//

std::string indri::index::DiskIndex::field( int fieldID ) {
  if( fieldID == 0 || fieldID > _fieldData.size() )
    return "";

  return _fieldData[fieldID-1].name;
}

//
// field
//

int indri::index::DiskIndex::field( const char* name ) {
  for( int i=0; i<_fieldData.size(); i++ ) {
    if( _fieldData[i].name == name )
      return i+1;
  }

  return 0;
}

//
// field
//

int indri::index::DiskIndex::field( const std::string& fieldName ) {
  return field( fieldName.c_str() );
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

//
// frequentVocabularyIterator
//

indri::index::VocabularyIterator* indri::index::DiskIndex::frequentVocabularyIterator() {
  // TODO: return appropriate data
  assert( 0 && "unimplemented" );
  return 0;
}

//
// infrequentVocabularyIterator
//

indri::index::VocabularyIterator* indri::index::DiskIndex::infrequentVocabularyIterator() {
  // TODO: return appropriate data
  assert( 0 && "unimplemented" );
  return 0;
}

