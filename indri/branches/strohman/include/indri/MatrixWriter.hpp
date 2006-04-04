
//
// MatrixWriter
//
// 2 March 2006 -- tds
//

#ifndef INDRI_MATRIXWRITER_HPP
#define INDRI_MATRIXWRITER_HPP

namespace indri {
  namespace similarity {
    class MatrixWriter {
    private:
      indri::file::File _file;
      indri::file::SequentialWriteBuffer* _buffer;

    public:
      void open( const std::string& filename ) {
        _file.create( filename );
        _buffer = new indri::file::SequentialWriteBuffer( _file, 1024*128 );
        _buffer->write( "IMAT", 4 );
      }

      void write( indri::similarity::Matrix* m ) {
        UINT64 rows = m->rows();
        rows = lemur_compat::htonll( rows );
        UINT64 columns = m->columns();
        columns = lemur_compat::htonll( columns );

        _buffer->write( &rows, 8 );
        _buffer->write( &columns, 8 );

        for( int i=0; i<m->rows(); i++ ) {
          for( int j=0; j<m->columns(); j++ ) {
            double value = m->get( i, j );
            value = lemur_compat::htond( value );
            _buffer->write( &value, 8 );
          }
        }
      }
      
      void close() {
        _buffer->flush();
        _file.close();
        delete _buffer;
      }
    };
  }
}

#endif // INDRI_MATRIXWRITER_HPP

