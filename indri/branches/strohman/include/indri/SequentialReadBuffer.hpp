
//
// SequentialReadBuffer
//
// 10 December 2004 -- tds
//

#ifndef INDRI_SEQUENTIALREADBUFFER_HPP
#define INDRI_SEQUENTIALREADBUFFER_HPP

#include "indri/indri-platform.h"
#include "indri/File.hpp"
#include "indri/InternalFileBuffer.hpp"

class SequentialReadBuffer {
private:
  File& _file;
  UINT64 _position;
  InternalFileBuffer _current;

public:
  SequentialReadBuffer( File& file ) :
    _file(file),
    _position(0),
    _current( 1024*1024 )
  {
  }

  SequentialReadBuffer( File& file, size_t length ) :
    _file(file),
    _position(0),
    _current( length )
  {
  }

  size_t read( void* buffer, size_t length ) {
    memcpy( buffer, read( length ), length );
    return length;
  }

  const void* peek( size_t length ) {
    const void* result = 0;
      
    if( _position >= _current.filePosition && (_position + length) <= _current.filePosition + _current.buffer.position() ) {
      result = _current.buffer.front() + ( _position - _current.filePosition );
    } else {
      // this isn't necessarily the most efficient way to do this, but it should work
      _current.buffer.clear();
      _current.filePosition = _position;
      _current.buffer.grow( length );

      int length = _file.read( _current.buffer.write( _current.buffer.size() ), _position, _current.buffer.size() );
      _current.buffer.unwrite( _current.buffer.size() - length );
      result = _current.buffer.front();
    }

    return result;
  }

  const void* read( size_t length ) {
    const void* result = peek( length );
    _position += length;
    return result;
  }

  void seek( UINT64 position ) {
    _position = position;
  }

  UINT64 position() {
    return _position;
  }
};

#endif // INDRI_SEQUENTIALREADBUFFER_HPP

