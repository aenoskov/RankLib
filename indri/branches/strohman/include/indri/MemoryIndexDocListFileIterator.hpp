
//
// MemoryIndexDocListFileIterator
//
// 23 November 2004 -- tds
//

#ifndef INDRI_MEMORYINDEXDOCLISTFILEITERATOR_HPP
#define INDRI_MEMORYINDEXDOCLISTFILEITERATOR_HPP

#include "indri/Mutex.hpp"
#include "indri/TermData.hpp"
#include "indri/DocListFileIterator.hpp"
#include "indri/DocListMemoryBuilder.hpp"
#include <algorithm>

namespace indri {
  namespace index {
    class MemoryIndexDocListFileIterator : public DocListFileIterator {
    private:
      const std::vector<MemoryIndex::term_entry*>& _termData;
      std::vector<MemoryIndex::term_entry*> _alphabetical;
      std::vector<MemoryIndex::term_entry*>::iterator _currentTerm;
      indri::index::DocListMemoryBuilderIterator _iterator;
      
    public:
      MemoryIndexDocListFileIterator( const std::vector<MemoryIndex::term_entry*>& termData ) :
        _termData(termData)
      {
      }

      void startIteration() {
        _alphabetical.clear();
        std::copy( _termData.begin(), _termData.end(), std::back_inserter( _alphabetical ) );
        assert( _alphabetical.size() );
        
        std::sort( _alphabetical.begin(), _alphabetical.end(), MemoryIndex::term_entry::term_less() );
        _currentTerm = _alphabetical.begin();
        _iterator.reset( (*_currentTerm)->list );
      }
      
      DocListIterator* currentEntry() { 
        return &_iterator;
      }
      
      bool nextEntry() {
        if( _currentTerm == _alphabetical.end() )
          return false;
        _currentTerm++;
        
        if( _currentTerm == _alphabetical.end() )
          return false;
        
        _iterator.reset( (*_currentTerm)->list );
      }
    };
  }
}

#endif // INDRI_MEMORYINDEXDOCLISTFILEITERATOR_HPP

