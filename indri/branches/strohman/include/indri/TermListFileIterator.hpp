
//
// TermListFileIterator
//
// 23 November 2004 -- tds
//

#ifndef INDRI_TERMLISTFILEITERATOR_HPP
#define INDRI_TERMLISTFILEITERATOR_HPP

namespace indri {
  namespace index {
    class TermListFileIterator {
    public:
      virtual ~TermListFileIterator();
      
      virtual void startIteration();
      virtual bool nextEntry();
      virtual bool finished();
      virtual TermList* currentEntry();
    };
  }
}

#endif // INDRI_TERMLISTFILEITERATOR_HPP

