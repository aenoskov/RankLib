
#include "indri/MemoryIndex.hpp"

const int ONE_MEGABYTE = 1024*1024;

void indri::index::MemoryIndex::_writeDocumentTermList( UINT64& offset, int& byteLength, int documentID, int documentLength, indri::index::TermListBuilder& locatedTerms ) {
  Buffer* addBuffer = 0;
  
  if( !_documentVectors.size() || _documentVectors.back()->size() - _documentVectors.back()->position() < documentLength ) {
    // we need a new Buffer
    if( !_documentVectors.size() )
      _documentVectorBaseOffset = 0;
    else
      _documentVectorBaseOffset += _documentVectors.back()->position();

    addBuffer = new Buffer(ONE_MEGABYTE);
    _documentVectors.push_back( addBuffer );
  }
  
  offset = _documentVectorBaseOffset + addBuffer->position();
  _termList.write( addBuffer );
  byteLength = addBuffer->position() + _documentVectorBaseOffset - offset;
}

void indri::index::MemoryIndex::_writeDocumentStatistics( File::offset_type offset, int byteLength, int indexedLength, int totalLength, int uniqueTerms ) {
  indri::index::DocumentData data;
  
  data.offset = offset;
  data.byteLength = byteLength;
  data.indexedLength = indexedLength;
  data.uniqueTermCount = uniqueTerms;
  
  _documentData.push_back( data );
}

void indri::index::MemoryIndex::_addOpenTags( greedy_vector<indri::index::FieldExtent>& indexedTags,
                               greedy_vector<indri::index::FieldExtent>& openTags,
                               const greedy_vector<TagExtent>& extents,
                               unsigned int& extentIndex, 
                               unsigned int position ) {
  for( ; extentIndex < extents.size(); extentIndex++ ) {
    const TagExtent* extent = &extents[extentIndex];
    
    if( extent->begin > position )
      break;
    
    int tagId = field( extent->name );
    
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

void indri::index::MemoryIndex::_lookupTerm( const char* term, int& termID, indri::index::TermData*& termData ) {
  term_cache_entry** entry = _cache->find( const_cast<char*>(term) );

  if( entry ) {
    // full cache hit
    termID = (*entry)->termID;
    termData = (*entry)->termData;
  } else {
    // this is a term we haven't seen before
    _corpusStatistics.uniqueTerms++;
    termID = _corpusStatistics.uniqueTerms;
    
    // create a term data structure
    termData = _createTermData();
    
    // store termData structure in the  [termID->termData] cache
    _idToTerm.push_back( termData );
    
    term_cache_entry** entry;
    term_cache_entry* newEntry = 0;
    int termLength = strlen(term);
    
    newEntry = (term_cache_entry*) malloc( termLength+1 + sizeof( newEntry->termID ) + sizeof( newEntry->termData ) );
    strcpy( newEntry->term, term );
    
    // store in [termString->termData] cache
    entry = _cache->insert( const_cast<char*>(newEntry->term) );
    *entry = newEntry;
    
    (*entry)->termID = termID;
    (*entry)->termData = termData;
    (*entry)->termData->term = newEntry->term;
  }
}

int indri::index::MemoryIndex::addDocument( ParsedDocument& document ) {
  ScopedLock sl( _lock );
  
  unsigned int position = 0;
  unsigned int extentIndex = 0;
  greedy_vector<indri::index::FieldExtent> openTags;
  greedy_vector<indri::index::FieldExtent> indexedTags;
  unsigned int indexedTerms = 0;
  greedy_vector<char*>& words = document.terms;

  // assign a document ID
  _totalDocuments++;
  int documentID = _totalDocuments;

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
    int wordID;
    indri::index::TermData* termData;

    _lookupTerm( word, wordID, termData );

    // store information about this term location
    _termList.addTerm( wordID );
    termData->list.addLocation( documentID, position ); 
    termData->corpus.addOccurrence( documentID );
    _seenTerms.push_back( termData );

    // update our open tag knowledge
    _addOpenTags( indexedTags, openTags, tagExtents, extentIndex, position );

    // for every open tag, we want to record that we've seen the 
    for( greedy_vector<indri::index::FieldExtent>::iterator tag = openTags.begin(); tag != openTags.end(); tag++ ) {
      indri::index::TermFieldStatistics* termField = &termData->fields[tag->id-1];
      termField->addOccurrence( documentID );

      indri::index::FieldStatistics* field = &_fieldData[tag->id-1]->statistics;
      field->addOccurrence( documentID );
    }

    _removeClosedTags( openTags, position );
    _corpusStatistics.totalTerms++;
    indexedTerms++;
  }

  // write field data out
  for( unsigned int i=0; i<indexedTags.size(); i++ ) {
    indri::index::FieldExtent& extent = indexedTags[i];
    _termList.addField( extent );
    _fieldData[extent.id-1]->list->addExtent( documentID, extent.begin, extent.end, extent.number );
  }

  File::offset_type offset;
  int byteLength;

  _writeDocumentTermList( offset, byteLength, documentID, int(words.size()), _termList );
  _writeDocumentStatistics( offset, byteLength, indexedTerms, int(words.size()), int(_seenTerms.size()) );

  _seenTerms.clear();
  return documentID;
}

//
// docListIterator
//

indri::index::DocListIterator* docListIterator( TERMID_T termID ) {
  assert( termID >= 0 );
  assert( termID < _corpusStatistics.uniqueTerms );
  
  if( termID == 0 )
    return 0;
  
  indri::index::TermData* termData = _idToTerm[termID];
  return termData->list.getIterator();
}

//
// docListIterator
//

indri::index::DocListIterator* docListIterator( const std::string& term ) {
  term_cache_entry** entry = _cache->find( const_cast<char*>(term.c_str()) );

  if( !entry )
    return 0;
  
  return (*entry)->termData->getIterator();
}  

//
// docListFileIterator
//

indri::index::DocListFileIterator* docListFileIterator() {
  // has to be in alphabetical order
  return indri::index::MemoryDocListFileIterator( _idToTerm );
}

//
// vocabularyIterator
//

indri::index::VocabularyIterator* vocabularyIterator() {
  return indri::index::MemoryIndexVocabularyIterator( _idToTerm );
}
