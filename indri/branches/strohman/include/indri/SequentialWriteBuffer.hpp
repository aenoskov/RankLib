
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
    flush();
    _position = position;
    _current.filePosition = position;
  }
  
  char* write( size_t length ) {
    // if this won't fit in the buffer, flush it
    if( _current.buffer.position() > 0 && _current.buffer.size() - _current.buffer.position() < length ) {
      flush();
      _current.filePosition = _position;
    }
    
    // write this data to the buffer, update file pointer, and get out
    _position += length;
    return _current.buffer.write( length );
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

  void flush() {
    _file.write( _current.buffer.front(), _current.filePosition, _current.buffer.position() );
    _current.buffer.clear();
    _current.filePosition = 0;
  }
};

#endif // INDRI_SEQUENTIALWRITEBUFFER_HPP

