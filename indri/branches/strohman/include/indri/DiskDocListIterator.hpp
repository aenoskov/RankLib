
//
// DiskDocListIterator
//
// 10 December 2004 -- tds
//

#ifndef INDRI_DISKDOCLISTITERATOR_HPP
#define INDRI_DISKDOCLISTITERATOR_HPP

#include "indri/DocListIterator.hpp"
#include "indri/SequentialReadBuffer.hpp"

namespace indri { 
  namespace index {
    class DiskDocListIterator : public DocListIterator {
    private:
      const char* _list;
      const char* _listEnd;
      int _skipDocument;

      SequentialReadBuffer* _file;
      UINT64 _startOffset;
      UINT64 _endOffset;

      bool _hasSkips;
      bool _hasTopdocs;

      greedy_vector<TopDocument> _topdocs;
      DocumentData _data;

      void _readEntry();
      void _readSkip();
      void _readTopdocs();

    public:
      DiskDocListIterator( SequentialReadBuffer* buffer, UINT64 startOffset, UINT64 endOffset );
      ~DiskDocListIterator();
      void setEndpoints( UINT64 startOffset, UINT64 endOffset );

      const greedy_vector<TopDocument>& topDocuments() = 0;

      void startIteration() = 0;
      bool nextEntry() = 0;
      bool nextEntry( int documentID ) = 0;
      DocumentData* currentEntry() = 0;
      bool finished() = 0;
    };
  }
}

#endif // INDRI_DISKDOCLISTITERATOR_HPP

