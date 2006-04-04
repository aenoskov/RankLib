
//
// MatrixReader
//
// 2 March 2006 -- tds
//

#ifndef INDRI_MATRIXREADER_HPP
#define INDRI_MATRIXREADER_HPP

#include "lemur/Exception.hpp"

namespace indri {
  namespace similarity {
    class MatrixReader {
    private:
      std::string _fileName;
      indri::file::File _file;
      indri::file::SequentialReadBuffer* _buffer;
      UINT64 _length;
      UINT64 _rows;
      UINT64 _columns;

      void _readHeader() {
        char text[4];
        _buffer->read( &text, 4 );
        if( memcmp( text, "IMAT", 4 ) )
            LEMUR_THROW( LEMUR_RUNTIME_ERROR, "Found a malformed IMAT file: " + _fileName );
      }

    public:
      void open( const std::string& fileName ) {
        _file.openRead( fileName );
        _buffer = new indri::file::SequentialReadBuffer( _file, 1024*128 );
        _fileName = fileName;
        _readHeader();
      }      

      void close() {
        _file.close();
        delete _buffer;
        _length = 0;
      }
      
      indri::similarity::Matrix* read() {
        // read the header, make sure this is an indri matrix
        UINT64 rows;
        UINT64 columns;

        _buffer->read( &rows, 8 );
        _buffer->read( &columns, 8 );

        rows = lemur_compat::ntohll( rows );
        columns = lemur_compat::ntohll( columns );

        indri::similarity::Matrix* m = new indri::similarity::Matrix( rows, columns );

        for( int i=0; i<rows; i++ ) {
          for( int j=0; j<columns; j++ ) {
            double value;
            _buffer->read( &value, 8 );
            value = lemur_compat::ntohd( value );
            m->set( i, j, value );
          }
        }

        return m;
      }
    };
  }
}

#endif // INDRI_MATRIXREADER_HPP

