
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
      bool _hasTopdocs;
      bool _isFrequent;

      greedy_vector<TopDocument> _topdocs;
      DocumentData _data;
      DocumentData* _result;

      void _readEntry();
      void _readSkip();
      void _readTopdocs();

    public:
      DiskDocListIterator( SequentialReadBuffer* buffer, UINT64 startOffset );
      ~DiskDocListIterator();
      void setStartOffset( UINT64 startOffset );

      const greedy_vector<TopDocument>& topDocuments();

      void startIteration();
      bool nextEntry();
      bool nextEntry( int documentID );
      DocumentData* currentEntry();
      bool finished();
      bool isFrequent() const;
    };
  }
}

#endif // INDRI_DISKDOCLISTITERATOR_HPP

