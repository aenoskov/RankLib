
//
// MemoryIndexVocabularyIterator
//
// 23 November 2004 -- tds
//

#ifndef INDRI_MEMORYINDEXVOCABULARYITERATOR_HPP
#define INDRI_MEMORYINDEXVOCABULARYITERATOR_HPP

#include "indri/Mutex.hpp"
#include "indri/TermData.hpp"

namespace indri {
  namespace index {
    class MemoryIndexVocabularyIterator : public VocabularyIterator {
    private:
      const std::vector<MemoryIndex::term_entry*>& _termData;
      std::vector<MemoryIndex::term_entry*> _alphabetical;
      std::vector<MemoryIndex::term_entry*>::iterator _iterator;
      
    public:
      MemoryIndexVocabularyIterator( const std::vector<MemoryIndex::term_entry*>& termData ) :
        _termData(termData)
      {
      }
      
      void startIteration() {
        _alphabetical.clear();
        std::copy( _termData.begin(), _termData.end(), std::back_inserter( _alphabetical ) );
        std::sort( _alphabetical.begin(), _alphabetical.end(), MemoryIndex::term_entry::term_less() );
        _iterator = _alphabetical.begin();
      }
      
      TermData* currentEntry() { 
        if( _iterator == _alphabetical.end() )
          return 0;
        
        return (*_iterator)->termData;
      }
      
      bool nextEntry() {
        if( _iterator == _alphabetical.end() )
          return false;
        
        _iterator++;
        
        if( _iterator == _alphabetical.end() )
          return false;
      }
      
      bool finished() {
        return _iterator == _alphabetical.end();
      }
    };
  }
}

#endif // INDRI_MEMORYINDEXVOCABULARYITERATOR_HPP

