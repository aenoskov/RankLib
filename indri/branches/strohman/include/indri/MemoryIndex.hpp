
//
// MemoryIndex
//
// 15 November 2004 -- tds
//

#ifndef INDRI_MEMORYINDEX_HPP
#define INDRI_MEMORYINDEX_HPP

#include "indri/Index.hpp"
#include "indri/Mutex.hpp"
#include "HashTable.hpp"
#include "indri/DocumentData.hpp"
#include "Buffer.hpp"
#include <list>
#include <vector>

#include "indri/DocListIterator.hpp"
#include "indri/DocListFileIterator.hpp"
#include "indri/TermList.hpp"
#include "indri/TermListFileIterator.hpp"
#include "indri/VocabularyIterator.hpp"
#include "indri/ParsedDocument.hpp"
#include "indri/DocListMemoryBuilder.hpp"
#include "indri/FieldStatistics.hpp"
#include "indri/CorpusStatistics.hpp"

namespace indri {
  namespace index {
    class MemoryIndex : public Index {
    private:
      Mutex _lock;

      CorpusStatistics _corpusStatistics;
      UINT64 _baseDocumentID;
      

      // vocabulary structure
      struct term_entry {
        char* term;
        int termID;
        indri::index::DocListMemoryBuilder list;
        TermData* termData;
      };

      // document buffers
      indri::index::TermList _termList;
      greedy_vector<term_entry*> _seenTerms;

      // term lookups
      HashTable<const char*, term_entry*> _stringToTerm;
      std::vector<term_entry*> _idToTerm;

      // field statistics
      HashTable<const char*, int> _fieldLookup;
      std::vector<FieldStatistics> _fieldData;
      
      // document statistics
      std::vector<indri::index::DocumentData> _documentData;
      
      // document vector buffers
      std::list<Buffer*> _documentVectors;
      UINT64 _documentVectorBaseOffset;
      
      void _addOpenTags( greedy_vector<indri::index::FieldExtent>& indexedTags,
                         greedy_vector<indri::index::FieldExtent>& openTags,
                         const greedy_vector<TagExtent>& extents,
                         unsigned int& extentIndex, 
                         unsigned int position );
      void _removeClosedTags( greedy_vector<indri::index::FieldExtent>& tags, unsigned int position );
      void _writeDocumentTermList( UINT64& offset, int& byteLength, int documentID, int documentLength, indri::index::TermList& locatedTerms );
      void _writeDocumentStatistics( UINT64 offset, int byteLength, int indexedLength, int totalLength, int uniqueTerms );
      term_entry* _lookupTerm( const char* term, int& termID );
      void _destroyTerms();

      int _fieldID( const std::string& fieldName );
      int _fieldID( const char* fieldName );


    public:


      UINT64 documentBase();
        
      UINT64 documentCount();
      UINT64 uniqueTermCount();
      
      UINT64 termCount( const std::string& term );
      UINT64 termCount();
      
      UINT64 fieldTermCount( const std::string& field );
      UINT64 fieldTermCount( const std::string& field, const std::string& term );
      
      UINT64 fieldDocumentCount( const std::string& field );
      UINT64 fieldDocumentCount( const std::string& field, const std::string& term );
      
      indri::index::DocListIterator* docListIterator( int termID );
      indri::index::DocListIterator* docListIterator( const std::string& term );
      indri::index::DocListFileIterator* docListFileIterator();
      indri::index::TermList* termList( int documentID );
      indri::index::TermListFileIterator* documentVectorIterator();
      indri::index::VocabularyIterator* vocabularyIterator();
      
      int addDocument( ParsedDocument& document );
    };
  }
}

#endif // INDRI_MEMORYINDEX_HPP
