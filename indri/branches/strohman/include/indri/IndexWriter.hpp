
//
// IndexWriter
//
// 26 November 2004 -- tds
//

#ifndef INDRI_INDEXWRITER_HPP
#define INDRI_INDEXWRITER_HPP

#include <vector>
#include <utility>
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
#include "indri/CorpusStatistics.hpp"
#include "indri/FieldStatistics.hpp"
#include "indri/TermBitmap.hpp"
#include "indri/TermRecorder.hpp"
#include "indri/TermTranslator.hpp"

struct WriterIndexContext {
  struct less {
  private:
    indri::index::DocListFileIterator::iterator_less _iterator_less;
  
  public:
    bool operator () ( const WriterIndexContext*& one, const WriterIndexContext*& two ) const {
      return _iterator_less( one->iterator, two->iterator );
    }
  };

  WriterIndexContext( indri::index::DocListFileIterator* iter, indri::index::Index* _index ) {
    iterator = iter;
    bitmap = new indri::index::TermBitmap;
    index = _index;
    infrequentIndex = 1;

    iterator->startIteration();
  }

  ~WriterIndexContext() {
    delete newlyFrequent;
    delete newlyInfrequent;
    delete bitmap;
  }

  indri::index::DocListFileIterator* iterator;
  indri::index::TermBitmap* bitmap;
  indri::index::Index* index;

  int infrequentIndex;
  indri::index::TermRecorder* newlyFrequent;
  indri::index::TermRecorder* newlyInfrequent;
};

typedef std::priority_queue<WriterIndexContext*,
                            std::vector<WriterIndexContext*>,
                            WriterIndexContext::less> invertedlist_pqueue;

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

      int _documentBase;
      indri::index::CorpusStatistics _corpus;
      std::vector<indri::index::Index::FieldDescription> _fields;
      std::vector<indri::index::FieldStatistics> _fieldData;

      void _writeManifest( const std::string& path );
      void _writeSkip( SequentialWriteBuffer* buffer, int document, int length );
      void _writeBatch( SequentialWriteBuffer* buffer, int document, int length, Buffer& data );

      void _writeFieldLists( std::vector<indri::index::Index*>& indexes, const std::string& path );
      void _writeFieldList( const std::string& fileName, std::vector<indri::index::DocExtentListIterator*> iterators );

      void _pushInvertedLists( greedy_vector<WriterIndexContext*>& lists, invertedlist_pqueue& queue );
      void _fetchMatchingInvertedLists( greedy_vector<WriterIndexContext*>& lists, invertedlist_pqueue& queue );
      void _writeStatistics( greedy_vector<WriterIndexContext*>& lists, indri::index::TermData* termData );
      void _writeInvertedLists( std::vector<indri::index::Index*>& indexes );
      void _storeTermEntry( IndexWriter::keyfile_pair& pair, indri::index::TermData* termData, INT64 startOffset, INT64 endOffset, int termID );
      void _storeFrequentTerms();
      void _addInvertedListData( greedy_vector<WriterIndexContext*>& lists, indri::index::TermData* termData, Buffer& listBuffer, UINT64& startOffset, UINT64& endOffset );
      void _storeMatchInformation( greedy_vector<WriterIndexContext*>& lists, int sequence, indri::index::TermData* termData, UINT64 startOffset, UINT64 endOffset );

      int _lookupTermID( Keyfile& keyfile, const char* term );
      void _writeDirectLists( std::vector<indri::index::Index*>& indexes, File& output );
      void _writeDirectLists( indri::index::Index* index, WriterIndexContext* context, SequentialWriteBuffer* output );


      indri::index::TermTranslator* _buildTermTranslator( Keyfile& newInfrequentTerms,
                                                          Keyfile& newFrequentTerms,
                                                          indri::index::TermRecorder& oldFrequentTermsRecorder,
                                                          indri::index::TermRecorder& newFrequentTermsRecorder,
                                                          indri::index::TermRecorder& newlyInfrequentTermsRecorder,
                                                          indri::index::TermBitmap* bitmap );

      enum {
        TOPDOCS_DOCUMENT_COUNT = 1000,
        FREQUENT_TERM_COUNT = 10000
      };

    public:
      IndexWriter();
      void write( indri::index::Index& index, const std::string& fileName );
    };
  }
}

#endif // INDRI_INDEXWRITER_HPP




 

