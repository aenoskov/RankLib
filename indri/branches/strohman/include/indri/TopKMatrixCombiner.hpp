
//
// TopKMatrixCombiner
//
// 2 March 2006 -- tds
//

#ifndef INDRI_TOPKMATRIXCOMBINER_HPP
#define INDRI_TOPKMATRIXCOMBINER_HPP

#include "indri/TopKMatrixReader.hpp"
#include "indri/TopKMatrixWriter.hpp"
#include "indri/delete_range.hpp"

namespace indri {
  namespace similarity {
    class TopKMatrixCombiner {
    private:
      std::vector<TopKMatrixReader*> _readers;
      TopKMatrixWriter _writer;
      int _k;

      class reader_small_document {
      public:
        bool operator() ( const TopKMatrixReader* one, const TopKMatrixReader* two ) const {
          return one->currentDocument() > two->currentDocument();
        }
      };

    public:
      TopKMatrixCombiner( const std::vector<TopKMatrixReader*>& readers, int k ) :
        _readers(readers),
        _writer(k)
      {
      }

      ~TopKMatrixCombiner() {
        indri::utility::delete_vector_contents( _readers );
      }

      void open( const std::string& outputFile ) {
        _writer.open( outputFile );
      }

      void close() {
        _writer.close();
      }

      void combine() {
        for( int i=0; i<_readers.size(); i++ ) {
          _readers[i]->readRow();
        }
  
        int currentDocument = 0;
        std::make_heap( _readers.begin(), _readers.end(), reader_small_document() );
        
        while( _readers.size() ) {
          std::pop_heap( _readers.begin(), _readers.end(), reader_small_document() );
          TopKMatrixReader* reader = _readers.back();

          if( currentDocument && currentDocument != reader->currentDocument() ) {
            _writer.writeRow( currentDocument );
          }

          currentDocument = reader->currentDocument();
          const indri::utility::greedy_vector< std::pair< int, double > >& row = reader->currentRow();

          for( int i=0; i<row.size(); i++ ) {
            _writer.push( row[i].first, row[i].second );
          }

          if( reader->finished() ) {
            _readers.erase( _readers.end()-1 );
            delete reader;
          } else {
            reader->readRow();
            std::push_heap( _readers.begin(), _readers.end(), reader_small_document() );
          }
        }

        _writer.writeRow( currentDocument );
      }
    };
  }
}

#endif // INDRI_TOPKMATRIXCOMBINER_HPP
