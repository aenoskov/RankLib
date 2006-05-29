
//
// TopKMatrixWriter
//
// 2 March 2006 -- tds
//

#ifndef INDRI_TOPKMATRIXWRITER_HPP
#define INDRI_TOPKMATRIXWRITER_HPP

namespace indri {
  namespace similarity {
    class TopKMatrixWriter {
    private:
      int _k;
      UINT64 _length;
      std::string _fileName;
      indri::file::File _file;
      indri::file::SequentialWriteBuffer* _buffer;
      indri::utility::greedy_vector< std::pair< double, int > > _rowBuffer;

      template<class S, class T>
      class pair_greater {
      public:
        bool operator() ( const std::pair<S,T>& one, const std::pair<S,T>& two ) const {
          return one > two;
        }
      };

      void _initRowBuffer() {
        _rowBuffer.clear();
        for( int i=0; i<_k; i++ ) {
          _rowBuffer.push_back( std::make_pair( -DBL_MAX, 0 ) );
        }
      }

    public:
      TopKMatrixWriter( int k ) :
      _k(k) {}

      void open( const std::string& fileName ) {
        _fileName = fileName;

        _file.create( fileName );
        _buffer = new indri::file::SequentialWriteBuffer( _file, 1024*128 );
        _buffer->write( "TOPK", 4 );
        int k = ntohl( _k );
        _buffer->write( &k, 4 );
        _initRowBuffer();
      }

      void close() {
        _buffer->flush();
        _file.close();
        delete _buffer;
      }

      void write( indri::similarity::Matrix* matrix, const std::vector<int>& source, const std::vector<int>& dest ) {
        assert( matrix->rows() == source.size() );
        assert( matrix->columns() == dest.size() );

        for( int i=0; i<matrix->rows(); i++ ) {
          for( int j=0; j<matrix->columns(); j++ ) {
            push( dest[j], matrix->get( i, j ) );
          }

          writeRow( source[i] );
        }
      }

      void push( int document, double score ) {
        assert( _rowBuffer.size() > 0 );
        if( (*_rowBuffer.begin()).first < score ) {
          std::pop_heap( _rowBuffer.begin(), _rowBuffer.end(), pair_greater<double,int>() );
          _rowBuffer.back() = std::make_pair( score, document );
          std::push_heap( _rowBuffer.begin(), _rowBuffer.end(), pair_greater<double,int>() );
        }
      }

      void writeRow( int sourceDocument ) {
        assert( _rowBuffer.size() == _k );
        _buffer->write( &sourceDocument, sizeof(UINT32) );
        for( int i=0; i<_rowBuffer.size(); i++ ) {
          UINT32 document = ntohl(_rowBuffer[i].second);
          double score = lemur_compat::ntohd(_rowBuffer[i].first);

          _buffer->write( &document, sizeof(UINT32) );
          _buffer->write( &score, sizeof(double) );
        }
        _initRowBuffer();
      }
    };
  }
}

#endif // INDRI_TOPKMATRIXWRITER_HPP

