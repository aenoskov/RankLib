
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
      DocListMemoryBuilderIterator _iterator;
      DocListData _data;
      
    public:
      MemoryIndexDocListFileIterator( const std::vector<MemoryIndex::term_entry*>& termData ) :
        _termData(termData)
      {
      }

      void startIteration() {
        _alphabetical.clear();
        std::copy( _termData.begin(), _termData.end(), std::back_inserter( _alphabetical ) );
        std::sort( _alphabetical.begin(), _alphabetical.end(), MemoryIndex::term_entry::term_less() );
        _currentTerm = _alphabetical.begin();
        _data.termData = 0;
        _data.iterator = 0;

        if( !finished() ) {
          _iterator.reset( (*_currentTerm)->list );
          _data.termData = (*_currentTerm)->termData;
          _data.iterator = &_iterator;
        }
      }
      
      bool finished() const {
        return _currentTerm == _alphabetical.end();
      }
      
      DocListData* currentEntry() { 
        return &_data;
      }
      
      const DocListData* currentEntry() const { 
        return &_data;
      }
      
      bool nextEntry() {
        if( _currentTerm == _alphabetical.end() )
          return false;
        _currentTerm++;
        
        if( _currentTerm == _alphabetical.end() )
          return false;
        
        _iterator.reset( (*_currentTerm)->list );
        _data.termData = (*_currentTerm)->termData;
        return true;
      }
    };
  }
}

#endif // INDRI_MEMORYINDEXDOCLISTFILEITERATOR_HPP

