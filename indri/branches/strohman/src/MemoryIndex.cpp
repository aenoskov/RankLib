
//
// MemoryIndex
//
// 24 November 2004 -- tds
//

#include "indri/MemoryIndex.hpp"
#include "indri/TermList.hpp"

#include "indri/MemoryIndexDocListFileIterator.hpp"
#include "indri/MemoryIndexVocabularyIterator.hpp"
#include "indri/MemoryIndexTermListFileIterator.hpp"

#include "indri/FieldStatistics.hpp"
#include "indri/ScopedLock.hpp"

#include "lemur/Keyfile.hpp"
#include "indri/greedy_vector"
#include "indri/delete_range.hpp"

const int ONE_MEGABYTE = 1024*1024;

//----------------------------
// Constructors
//----------------------------

indri::index::MemoryIndex::MemoryIndex() : _stringToTerm( 1024 * 1024 ) {
  _baseDocumentID = 0;
  _termListsBaseOffset = 0;
}

indri::index::MemoryIndex::MemoryIndex( int docBase ) : _stringToTerm( 1024 * 1024 ) {
  _baseDocumentID = docBase;
  _termListsBaseOffset = 0;
}

indri::index::MemoryIndex::MemoryIndex( int docBase, const std::vector<Index::FieldDescription>& fields ) : _stringToTerm( 1024 * 1024 ) {
  _baseDocumentID = docBase;
  _termListsBaseOffset = 0;

  HashTable<const char*, int> _fieldLookup;
  std::vector<FieldStatistics> _fieldData;
  std::vector<indri::index::DocExtentListMemoryBuilder*> _fieldLists;

  for( size_t i=0; i<fields.size(); i++ ) {
    int fieldID = i+1;

    _fieldData.push_back( FieldStatistics( fields[i].name, fields[i].numeric, 0, 0 ) );
    _fieldLists.push_back( new DocExtentListMemoryBuilder( fields[i].numeric ) );
    _fieldLookup.insert( _fieldData.back().name.c_str(), fieldID );
  }
}

//----------------------------
// Destructors
//----------------------------

indri::index::MemoryIndex::~MemoryIndex() {
  // delete term lists
  std::list<Buffer*>::iterator bufferIter;
  for( bufferIter = _termLists.begin(); bufferIter != _termLists.end(); bufferIter++ ) {
    delete *bufferIter; 
  }

  // delete field lists
  delete_vector_contents<DocExtentListMemoryBuilder*>( _fieldLists );

  // delete term entries
  _destroyTerms();
}

//
// close
//

void indri::index::MemoryIndex::close() {
  // does nothing
}

// ---------------------------
// Corpus statistics accessors
// ---------------------------

//
// documentBase
//

int indri::index::MemoryIndex::documentBase() {
  return _baseDocumentID;
}

//
// documentLength
//

int indri::index::MemoryIndex::documentLength( int documentID ) {
  return _documentData[ documentID - _baseDocumentID ].indexedLength;
}

//
// term
//

int indri::index::MemoryIndex::term( const char* term ) {
  term_entry** entry = _stringToTerm.find( term );

  if( entry )
    return (*entry)->termID;

  return 0;
}

//
// term
//

int indri::index::MemoryIndex::term( const std::string& term ) {
  term_entry** entry = _stringToTerm.find( term.c_str() );

  if( entry )
    return (*entry)->termID;

  return 0;
}

//
// term
//

std::string indri::index::MemoryIndex::term( int termID ) {
  if( termID <= 0 || termID >= _idToTerm.size() )
    return std::string();

  term_entry* entry = _idToTerm[ termID ];
  return entry->term;
}

//
// field
//

std::string indri::index::MemoryIndex::field( int fieldID ) {
  if( fieldID <= 0 || fieldID > _fieldData.size() )
    return "";
  
  return _fieldData[fieldID-1].name;
}

//
// field
//

int indri::index::MemoryIndex::field( const std::string& fieldName ) {
  return field( fieldName.c_str() );
}

//
// field
//

int indri::index::MemoryIndex::field( const char* fieldName ) {
  return _fieldID( fieldName );
}

//
// documentCount
//

UINT64 indri::index::MemoryIndex::documentCount( const std::string& term ) {
  term_entry** entry = _stringToTerm.find( term.c_str() );
  return (*entry)->termData->corpus.documentCount;
}

//
// fieldDocumentCount
//

UINT64 indri::index::MemoryIndex::fieldDocumentCount( const std::string& field, const std::string& term ) {
  term_entry** entry = _stringToTerm.find( term.c_str() );
  int id = _fieldID( field );

  if( !entry || id == 0 )
    return 0;

  return (*entry)->termData->fields[id].documentCount;
}

//
// fieldDocumentCount
//

UINT64 indri::index::MemoryIndex::fieldDocumentCount( const std::string& field ) {
  int id = _fieldID( field );

  if( id == 0 )
    return 0;

  return _fieldData[id].documentCount;
}

//
// fieldTermCount
//

UINT64 indri::index::MemoryIndex::fieldTermCount( const std::string& field ) {
  int id = _fieldID( field );

  if( id == 0 )
    return 0;

  return _fieldData[id].totalCount;
}

//
// fieldTermCount
//

UINT64 indri::index::MemoryIndex::fieldTermCount( const std::string& field, const std::string& term ) {
  term_entry** entry = _stringToTerm.find( term.c_str() );
  int id = _fieldID( field );

  if( !entry || id == 0 )
    return 0;

  return (*entry)->termData->fields[id].totalCount;
}

//
// termCount
//

UINT64 indri::index::MemoryIndex::termCount() {
  return _corpusStatistics.totalTerms;
}

//
// termCount
//

UINT64 indri::index::MemoryIndex::termCount( const std::string& term ) {
  return _corpusStatistics.totalTerms;
}

//
// uniqueTermCount
//

UINT64 indri::index::MemoryIndex::uniqueTermCount() {
  return _corpusStatistics.uniqueTerms;
}

//
// documentCount
//

UINT64 indri::index::MemoryIndex::documentCount() {
  return _corpusStatistics.totalDocuments;
}

//
// _fieldID
//

int indri::index::MemoryIndex::_fieldID( const char* fieldName ) {
  int* entry = _fieldLookup.find( fieldName );

  if( entry )
    return *entry;

  return 0;
}

//
// _fieldID
//

int indri::index::MemoryIndex::_fieldID( const std::string& fieldName ) {
  return _fieldID( fieldName.c_str() );
}

//
// _writeFieldExtents
//

void indri::index::MemoryIndex::_writeFieldExtents( int documentID, greedy_vector<indri::index::FieldExtent>& indexedTags ) {
  // write field data out
  for( unsigned int i=0; i<indexedTags.size(); i++ ) {
    indri::index::FieldExtent& extent = indexedTags[i];
    _termList.addField( extent );
    _fieldLists[extent.id-1]->addLocation( documentID, extent.begin, extent.end, extent.number );
  }
}

//
// _writeDocumentTermList
//

void indri::index::MemoryIndex::_writeDocumentTermList( UINT64& offset, int& byteLength, int documentID, int documentLength, indri::index::TermList& locatedTerms ) {
  Buffer* addBuffer = 0;
  int docDataLength = 10 + 5 * locatedTerms.terms().size() + 2 * sizeof(FieldExtent) * locatedTerms.fields().size();
  
  if( !_termLists.size() || _termLists.back()->size() - _termLists.back()->position() < docDataLength ) {
    // we need a new Buffer
    if( !_termLists.size() )
      _termListsBaseOffset = 0;
    else
      _termListsBaseOffset += _termLists.back()->position();

    addBuffer = new Buffer(ONE_MEGABYTE);
    _termLists.push_back( addBuffer );
  } else {
    addBuffer = _termLists.back();
  }
  
  offset = _termListsBaseOffset + addBuffer->position();
  _termList.write( *addBuffer );
  byteLength = addBuffer->position() + _termListsBaseOffset - offset;
}

//
// _writeDocumentStatistics
//

void indri::index::MemoryIndex::_writeDocumentStatistics( UINT64 offset, int byteLength, int indexedLength, int totalLength, int uniqueTerms ) {
  indri::index::DocumentData data;
  
  data.offset = offset;
  data.byteLength = byteLength;
  data.indexedLength = indexedLength;
  data.uniqueTermCount = uniqueTerms;
  
  _documentData.push_back( data );
}

//
// _addOpenTags
//

void indri::index::MemoryIndex::_addOpenTags( greedy_vector<indri::index::FieldExtent>& indexedTags,
                               greedy_vector<indri::index::FieldExtent>& openTags,
                               const greedy_vector<TagExtent>& extents,
                               unsigned int& extentIndex, 
                               unsigned int position ) {
  for( ; extentIndex < extents.size(); extentIndex++ ) {
    const TagExtent* extent = &extents[extentIndex];
    
    if( extent->begin > position )
      break;
    
    int tagId = _fieldID( extent->name );
    
    if( tagId == 0 )
      continue;
    
    indri::index::FieldExtent converted( tagId, extent->begin, extent->end, extent->number );
    
    openTags.push_back( converted );
    indexedTags.push_back( converted );
  }
}

//
// _removeClosedTags
//

void indri::index::MemoryIndex::_removeClosedTags( greedy_vector<indri::index::FieldExtent>& tags, unsigned int position ) {
  for( unsigned int i=0; i<tags.size(); ) {
    if( tags[i].end <= position ) {
      tags.erase( tags.begin() + i );
    } else {
      i++;
    }
  }
}

//
// _lookupTerm
//
// Tries to find this term in a hash table--if it isn't there, it gets added.
//

indri::index::MemoryIndex::term_entry* indri::index::MemoryIndex::_lookupTerm( const char* term ) {
  term_entry** entry = _stringToTerm.find( term );

  // if we've seen it, return it
  if( entry )
    return *entry;

  // this is a term we haven't seen before
  _corpusStatistics.uniqueTerms++;
  int termID = _corpusStatistics.uniqueTerms;
  
  // create a term data structure
  TermData* termData = termdata_create( _fieldData.size() );
  
  term_entry* newEntry = 0;
  int termLength = strlen(term);
  
  newEntry = (term_entry*) malloc( termLength+1 + sizeof(term_entry) );
  newEntry->term = (char*) newEntry + sizeof(term_entry);
  strcpy( newEntry->term, term );
  new (newEntry) term_entry;
  
  // store in [termString->termData] cache
  entry = _stringToTerm.insert( newEntry->term );
  *entry = newEntry;

  // store termData structure in the  [termID->termData] cache
  _idToTerm.push_back( newEntry );
  
  newEntry->termID = termID;
  newEntry->termData = termData;
  newEntry->termData->term = newEntry->term;

  return newEntry;
}

//
// _destroyTerms
//

void indri::index::MemoryIndex::_destroyTerms() {
  for( unsigned int i=0; i<_idToTerm.size(); i++ ) {
    term_entry* entry = _idToTerm[i];
    termdata_delete( entry->termData, _fieldData.size() );
    entry->~term_entry();
    free( entry );
  }
}

//
// addDocument
//

int indri::index::MemoryIndex::addDocument( ParsedDocument& document ) {
  ScopedLock sl( _lock );
  
  unsigned int position = 0;
  unsigned int extentIndex = 0;
  greedy_vector<indri::index::FieldExtent> openTags;
  greedy_vector<indri::index::FieldExtent> indexedTags;
  unsigned int indexedTerms = 0;
  greedy_vector<char*>& words = document.terms;

  // assign a document ID -- TODO: check for off by one error here
  _corpusStatistics.totalDocuments++;
  int documentID = _baseDocumentID + _corpusStatistics.totalDocuments;

  _termList.clear();

  // move words into inverted lists, recording model statistics as we go
  for( position = 0; position < words.size(); position++ ) {
    const char* word = words[position];
    
    if( !word || *word == 0 ) {
      _termList.addTerm(0);
      continue;
    }

    int wordLength = strlen(word);

    if( wordLength >= Keyfile::MAX_KEY_LENGTH-1 ) {
      _termList.addTerm(0);     
      continue;
    }

    // fetch everything we know about this word so far
    term_entry* entry = _lookupTerm( word );

    // store information about this term location
    _termList.addTerm( entry->termID );
    entry->list.addLocation( documentID, position ); 
    entry->termData->corpus.addOccurrence( documentID );
    _seenTerms.push_back( entry );

    // update our open tag knowledge
    _addOpenTags( indexedTags, openTags, document.tags, extentIndex, position );

    // for every open tag, we want to record that we've seen the 
    for( greedy_vector<indri::index::FieldExtent>::iterator tag = openTags.begin(); tag != openTags.end(); tag++ ) {
      indri::index::TermFieldStatistics* termField = &entry->termData->fields[tag->id-1];
      termField->addOccurrence( documentID );

      indri::index::FieldStatistics* field = &_fieldData[tag->id-1];
      field->addOccurrence( documentID );
    }

    _removeClosedTags( openTags, position );
    _corpusStatistics.totalTerms++;
    indexedTerms++;
  }

  _writeFieldExtents( documentID, indexedTags );

  UINT64 offset;
  int byteLength;

  _writeDocumentTermList( offset, byteLength, documentID, int(words.size()), _termList );
  _writeDocumentStatistics( offset, byteLength, indexedTerms, int(words.size()), int(_seenTerms.size()) );

  _seenTerms.clear();
  return documentID;
}

//
// docListIterator
//

indri::index::DocListIterator* indri::index::MemoryIndex::docListIterator( int termID ) {
  assert( termID >= 0 );
  assert( termID < _corpusStatistics.uniqueTerms );
  
  if( termID == 0 )
    return 0;
  
  term_entry* entry = _idToTerm[termID];
  return entry->list.getIterator();
}

//
// docListIterator
//

indri::index::DocListIterator* indri::index::MemoryIndex::docListIterator( const std::string& term ) {
  term_entry** entry = _stringToTerm.find( term.c_str() );

  if( !entry )
    return 0;
  
  return (*entry)->list.getIterator();
}  

//
// fieldListIterator
//

indri::index::DocExtentListIterator* indri::index::MemoryIndex::fieldListIterator( int fieldID ) {
  if( fieldID <= 0 || fieldID >= _fieldData.size() )
    return 0;
  
  DocExtentListMemoryBuilder* builder = _fieldLists[fieldID-1];
  return new DocExtentListMemoryBuilderIterator( *builder );
}

//
// fieldListIterator
//

indri::index::DocExtentListIterator* indri::index::MemoryIndex::fieldListIterator( const std::string& field ) {
  int fieldID = _fieldID( field );
  if( fieldID <= 0 || fieldID >= _fieldData.size() )
    return 0;
  
  DocExtentListMemoryBuilder* builder = _fieldLists[fieldID-1];
  return new DocExtentListMemoryBuilderIterator( *builder );
}

//
// termList
//

const indri::index::TermList* indri::index::MemoryIndex::termList( int documentID ) {
  int documentIndex = documentID - documentBase();
  if( documentIndex < 0 || documentIndex >= _documentData.size() )
    return 0;

  const DocumentData& data = _documentData[documentIndex];
  UINT64 documentOffset = data.offset;
  Buffer* documentBuffer = 0;
  std::list<Buffer*>::const_iterator iter;

  for( iter = _termLists.begin(); iter != _termLists.end(); ++iter ) {
    if( documentOffset < (*iter)->position() ) {
      documentBuffer = (*iter);
      break;
    }

    documentOffset -= (*iter)->position();
  }

  assert( documentBuffer );
  TermList* list = new TermList();

  list->read( documentBuffer->front() + documentOffset, data.byteLength );
  return list;
}

//
// termListFileIterator
//

indri::index::TermListFileIterator* indri::index::MemoryIndex::termListFileIterator() {
  return new MemoryIndexTermListFileIterator( _termLists, _documentData );
}

//
// docListFileIterator
//

indri::index::DocListFileIterator* indri::index::MemoryIndex::docListFileIterator() {
  // has to be in alphabetical order
  return new indri::index::MemoryIndexDocListFileIterator( _idToTerm );
}

//
// vocabularyIterator
//

indri::index::VocabularyIterator* indri::index::MemoryIndex::vocabularyIterator() {
  return new indri::index::MemoryIndexVocabularyIterator( _idToTerm );
}

//
// infrequentVocabularyIterator
//

indri::index::VocabularyIterator* indri::index::MemoryIndex::infrequentVocabularyIterator() {
  return vocabularyIterator();
}

//
// frequentVocabularyIterator
//

indri::index::VocabularyIterator* indri::index::MemoryIndex::frequentVocabularyIterator() {
  return 0;
}
