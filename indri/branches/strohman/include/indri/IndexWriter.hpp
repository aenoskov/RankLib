
//
// IndexWriter
//
// 26 November 2004 -- tds
//

#ifndef INDRI_INDEXWRITER_HPP
#define INDRI_INDEXWRITER_HPP

#include <vector>
#include <queue>

#include "lemur/lemur-compat.hpp"
#include "indri/indri-platform.h"
#include "indri/greedy_vector"
#include "indri/TermData.hpp"
#include "lemur/Keyfile.hpp"
#include "indri/Index.hpp"
#include "indri/VocabularyMap.hpp"
#include "indri/DocListFileIterator.hpp"
#include "indri/File.hpp"
#include "indri/SequentialWriteBuffer.hpp"

struct WriterInvertedList {
  struct less {
  private:
    indri::index::DocListFileIterator::iterator_less _iterator_less;
  
  public:
    bool operator () ( const WriterInvertedList*& one, const WriterInvertedList*& two ) const {
      return _iterator_less( one->iterator, two->iterator );
    }
  };

  WriterInvertedList( indri::index::DocListFileIterator* iter, indri::index::Index* _index ) {
    iterator = iter;
    matches = 0;
    index = _index;

    iterator->startIteration();
  }

  indri::index::DocListFileIterator* iterator;
  indri::index::VocabularyMap* matches;
  indri::index::Index* index;
};

typedef std::priority_queue<WriterInvertedList*,
                            std::vector<WriterInvertedList*>,
                            WriterInvertedList::less> invertedlist_pqueue;

namespace indri {
  namespace index {
    class IndexWriter {
    private:
      struct top_term_entry {
        struct greater {
          bool operator () ( top_term_entry& one, top_term_entry& two ) {
            return one.termData->corpus.totalCount > two.termData->corpus.totalCount;
          }
        };

        indri::index::TermData* termData;
        UINT64 startOffset;
        UINT64 endOffset;
      };

      struct keyfile_pair {
        Keyfile* stringMap;
        Keyfile* idMap;
      };

      keyfile_pair _infrequentTerms;
      keyfile_pair _frequentTerms;

      File _documentStatistics;
      File _documentLengths;

      File _invertedFile;
      File _directFile;

      SequentialWriteBuffer* _invertedOutput;

      greedy_vector<top_term_entry> _topTerms;
      Buffer _termDataBuffer;

      int _fieldCount;

      void _pushInvertedLists( greedy_vector<WriterInvertedList*>& lists, invertedlist_pqueue& queue );
      void _fetchMatchingInvertedLists( greedy_vector<WriterInvertedList*>& lists, invertedlist_pqueue& queue );
      void _writeStatistics( greedy_vector<WriterInvertedList*>& lists, indri::index::TermData* termData );
      void _writeInvertedLists( std::vector<indri::index::Index*>& indexes );
      void _storeTermEntry( IndexWriter::keyfile_pair& pair, indri::index::TermData* termData, INT64 startOffset, INT64 endOffset, int termID );
      void _storeFrequentTerms();
      void _addInvertedListData( greedy_vector<WriterInvertedList*>& lists, indri::index::TermData* termData, Buffer& listBuffer, UINT64& startOffset, UINT64& endOffset );
      void _storeMatchInformation( int sequence, indri::index::TermData* termData, UINT64 startOffset, UINT64 endOffset );

    public:
      IndexWriter( int fields );
      void write( indri::index::Index& index, const std::string& fileName );
    };
  }
}

#endif // INDRI_INDEXWRITER_HPP




 

