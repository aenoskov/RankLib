

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
    class MemoryIndexVocabularyIterator {
    private:
      std::vector<indri::index::TermData*>& _termData;
      std::vector<indri::index::TermData*>& _alphabetical;
      std::vector<indri::index::TermData*>::iterator _iterator;
      
    public:
      MemoryIndexVocabularyIterator( std::vector<indri::index::TermData*>& termData ) :
        _termData(termData)
      {
      }
      
      void startIteration() {
        _alphabetical.clear();
        std::copy( _termData.begin(), _termData.end(), std::back_inserter( _alphabetical ) );
        std::sort( _alphabetical.begin(), _alphabetical.end(), TermData::term_less() );
        _iterator = _alphabetical.begin();
      }
      
      const TermData* current() { 
        if( _iterator == _alphabetical.end() )
          return 0;
        
        return *_iterator;
      }
      
      bool next() {
        if( _iterator == _alphabetical.end() )
          return false;
        
        _iterator++;
        
        if( _iterator == _alphabetical.end() )
          return false;
      }
    };
  }
}

#endif // INDRI_MEMORYINDEXVOCABULARYITERATOR_HPP

