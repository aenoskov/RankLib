
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
#include <algorithm>

namespace indri {
  namespace index {
    class MemoryIndexDocListFileIterator : public DocListFileIterator {
    private:
      std::vector<indri::index::TermData*>& _termData;
      std::vector<indri::index::TermData*>  _alphabetical;
      std::vector<indri::index::TermData*>::iterator _currentTerm;
      indri::index::DocListMemoryBuilderIterator _iterator;
      
    public:
      MemoryIndexDocListFileIterator( std::vector<indri::index::TermData*>& termData ) :
        _termData(termData)
      {
      }

      void startIteration() {
        _alphabetical.clear();
        std::copy( _termData.begin(), _termData.end(), std::back_inserter( _alphabetical ) );
        assert( _alphabetical.size() );
        
        std::sort( _alphabetical.begin(), _alphabetical.end(), TermData::term_less() );
        _currentTerm = _alphabetical.begin();
        _iterator.reset( _currentTerm );
      }
      
      const DocListIterator* current() { 
        return &_iterator;
      }
      
      bool next() {
        if( _currentTerm == _alphabetical.end() )
          return false;
        _currentTerm++;
        
        if( _currentTerm == _alphabetical.end() )
          return false;
        
        _iterator.reset( _currentTerm );
      }
    };
  }
}

#endif // INDRI_MEMORYINDEXDOCLISTFILEITERATOR_HPP

