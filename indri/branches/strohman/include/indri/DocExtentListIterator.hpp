
//
// DocExtentListIterator
//
// 24 November 2004 -- tds
//

#ifndef INDRI_DOCEXTENTLISTITERATOR_HPP
#define INDRI_DOCEXTENTLISTITERATOR_HPP

#include "Extent.hpp"
#include "indri/indri-platform.h"

namespace indri {
  namespace index {
    class DocExtentListIterator {
    public:
      struct DocumentExtentData {
        int document;
        greedy_vector<Extent> extents;
        greedy_vector<INT64> numbers;
      };

      void startIteration();
      bool nextEntry();
      bool nextEntry( int documentID );
      DocumentExtentData* currentEntry();
    };
  }
}

#endif // INDRI_DOCEXTENTLISTITERATOR_HPP
