
//
// VocabularyIterator
//
// 23 November 2004 -- tds
//

#ifndef INDRI_VOCABULARYITERATOR_HPP
#define INDRI_VOCABULARYITERATOR_HPP

#include "indri/TermData.hpp"

namespace indri {
  namespace index { 
    class VocabularyIterator {
    public:
      void startIteration();
      bool finished();
      bool nextEntry();
      TermData* currentEntry();
    };
  }
}
    
#endif // INDRI_VOCABULARYITERATOR_HPP



