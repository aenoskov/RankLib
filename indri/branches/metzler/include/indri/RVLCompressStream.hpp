/*==========================================================================
 * Copyright (c) 2004 University of Massachusetts.  All Rights Reserved.
 *
 * Use of the Lemur Toolkit for Language Modeling and Information Retrieval
 * is subject to the terms of the software license set forth in the LICENSE
 * file included with this software, and also available at
 * http://www.lemurproject.org/license.html
 *
 *==========================================================================
*/


//
// RVLCompressStream
//
// 9 February 2004 -- tds
//

#ifndef INDRI_RVLCOMPRESSSTREAM_HPP
#define INDRI_RVLCOMPRESSSTREAM_HPP

class RVLCompressStream {
private:
  char* _buffer;
  size_t _bufferSize;
  char* _current;

public:
  RVLCompressStream( char* buffer, size_t size ) {
    _buffer = buffer;
    _bufferSize = size;
    _current = _buffer;
  }

  const char* data() const {
    return _buffer;
  }

  size_t dataSize() const {
    return _current - _buffer;
  }

  RVLCompressStream& operator<< ( int value ) {
    _current = RVLCompress::compress_int( _current, value );
    assert( _bufferSize >= size_t(_current - _buffer) );
    return *this;
  }

  RVLCompressStream& operator<< ( unsigned int value ) {
    _current = RVLCompress::compress_int( _current, value );
    assert( _bufferSize >= size_t(_current - _buffer) );
    return *this;
  }

  RVLCompressStream& operator<< ( INT64 value ) {
    _current = RVLCompress::compress_longlong( _current, value );
    assert( _bufferSize >= size_t(_current - _buffer) );
    return *this;
  }

  RVLCompressStream& operator << ( float value ) {
    // can't compress a float, unfortunately
    memcpy( _current, &value, sizeof value );
    _current += sizeof value;
    return *this;
  }
};

#endif // INDRI_RVLCOMPRESSSTREAM_HPP

