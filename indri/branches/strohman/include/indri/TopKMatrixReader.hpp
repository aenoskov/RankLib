
//
// TopKMatrixReader
//
// 2 March 2006 -- tds
//

#ifndef INDRI_TOPKMATRIXREADER_HPP
#define INDRI_TOPKMATRIXREADER_HPP

#include "lemur/Exception.hpp"
#include "indri/greedy_vector"

namespace indri {
  namespace similarity {
    class TopKMatrixReader {
    private:
      int _k;
      UINT64 _length;
      std::string _fileName;
      indri::file::File _inFile;
      indri::file::SequentialReadBuffer* _inBuffer;

      int _document;
      indri::utility::greedy_vector< std::pair< int, double > > _list;

      void _readHeader() {
        char text[4];
        _inBuffer->read( &text, 4 );
        if( memcmp( text, "TOPK", 4 ) )
          LEMUR_THROW( LEMUR_RUNTIME_ERROR, "Found a malformed TOPK file: " + _fileName );

        _inBuffer->read( &_k, sizeof(UINT32) );
        _k = htonl( _k );
      }

    public:
      void open( const std::string& filename ) {
        _fileName = filename;

        _inFile.openRead( filename );
        _inBuffer = new indri::file::SequentialReadBuffer( _inFile, 1024*128 );
        _length = _inFile.size();

        _readHeader();
      }

      ~TopKMatrixReader() {
        close();
      }

      void close() {
        delete _inBuffer;
        _inBuffer = 0;
        _inFile.close();
      }

      const indri::utility::greedy_vector< std::pair< int, double > >& readRow() {
        // BUGBUG: need to byte swap this
        _inBuffer->read( &_document, sizeof(UINT32) );
        _list.clear();

        for( int i=0; i<_k; i++ ) {
          int otherDocID;
          double similarity;

          _inBuffer->read( &otherDocID, sizeof(UINT32) );
          _inBuffer->read( &similarity, sizeof(double) );
          otherDocID = htonl( otherDocID );
          similarity = lemur_compat::htond( similarity );

          _list.push_back( std::make_pair( otherDocID, similarity ) );
        }

        return _list;
      }

      int currentDocument() const {
        return _document;
      }

      const indri::utility::greedy_vector< std::pair< int, double > >& currentRow() const {
        return _list;
      }

      bool finished() const {
        return _length <= _inBuffer->position();
      }
    };
  }
}

#endif // INDRI_TOPKMATRIXREADER_HPP
