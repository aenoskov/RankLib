
//
// SequentialWriteBuffer
//
// 15 November 2004 -- tds
//

#ifndef INDRI_SEQUENTIALWRITEBUFFER_HPP
#define INDRI_SEQUENTIALWRITEBUFFER_HPP

#include "indri/File.hpp"
#include "indri/InternalFileBuffer.hpp"

class SequentialWriteBuffer {
private:
  File& _file;
  InternalFileBuffer _current;
  UINT64 _position;

public:
  SequentialWriteBuffer( File& file, size_t length ) :
    _file(file),
    _current(length),
    _position(0)
  {
  }

  void seek( UINT64 position ) {
    if( position < _current.filePosition ) {
      flush();
      _current.filePosition = position;
    }

    _position = position;
  }
  
  char* write( size_t length ) {
    assert( _position >= _current.filePosition );

    UINT64 endBuffer = _current.filePosition + _current.buffer.size();
    UINT64 endBufferData = _current.filePosition + _current.buffer.position();
    UINT64 endWrite = length + _position;
    char* writeSpot;

    if( endBuffer < endWrite ) {
      // it's not going to fit without flushing
      flush();
      _current.filePosition = _position;

      endBuffer = _current.filePosition + _current.buffer.size();
      endBufferData = _current.filePosition + _current.buffer.position();
    }

    if( endWrite > endBufferData ) {
      // need to move the buffer pointer to the end, potentially resizing buffer
      _current.buffer.write( endWrite - endBufferData );
    }

    writeSpot = _current.buffer.front() + (_position - _current.filePosition);
    assert( writeSpot + length <= _current.buffer.front() + _current.buffer.position() );
    _position += length;

    return writeSpot;
  }

  void write( const void* buffer, size_t length ) {
    memcpy( write( length ), buffer, length );
  }
  
  void unwrite( size_t length ) {
    assert( length <= _current.buffer.position() );
    _current.buffer.unwrite( length );
    _position -= length;
  }
  
  UINT64 tell() const {
    return _position;
  }

  void flushBytes( UINT64 bytes ) {
    if( bytes != 0 ) {
      assert( bytes <= _current.buffer.position() );
      _file.write( _current.buffer.front(), _current.filePosition, bytes );
      _current.buffer.remove( bytes );
      _current.filePosition += bytes;
    }
  }

  void flush() {
    flushBytes( _current.buffer.position() );
  }
};

#endif // INDRI_SEQUENTIALWRITEBUFFER_HPP

