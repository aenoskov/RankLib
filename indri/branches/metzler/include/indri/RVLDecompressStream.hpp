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
//
//
//
//

#ifndef INDRI_RVLDECOMPRESSSTREAM_HPP
#define INDRI_RVLDECOMPRESSSTREAM_HPP

class RVLDecompressStream {
private:
  const char* _buffer;
  int _bufferSize;
  const char* _current;

public:
  RVLDecompressStream( const char* buffer, int size ) {
    _buffer = buffer;
    _bufferSize = size;
    _current = buffer;
  }

  RVLDecompressStream& operator>> ( INT64& value ) {
    _current = RVLCompress::decompress_longlong( _current, value );
    assert( _current - _buffer <= _bufferSize );
    return *this;
  }

  RVLDecompressStream& operator>> ( UINT64& value ) {
    INT64 other;
    _current = RVLCompress::decompress_longlong( _current, other );
    assert( _current - _buffer <= _bufferSize );
    value = other;
    return *this;
  }

  RVLDecompressStream& operator>> ( int& value ) {
    _current = RVLCompress::decompress_int( _current, value );
    assert( _current - _buffer <= _bufferSize );    
    return *this;
  }

  RVLDecompressStream& operator>> ( unsigned int& value ) {
    int v;
    _current = RVLCompress::decompress_int( _current, v );
    value = (unsigned int) v;
    assert( _current - _buffer <= _bufferSize );    
    return *this;
  }

  RVLDecompressStream& operator>> ( float& value ) {
    // doubles aren't compressed
    memcpy( &value, _current, sizeof value );
    _current += sizeof value;
    return *this;
  }

  bool done() const {
    return (_current - _buffer) >= _bufferSize;
  }
};

#endif // INDRI_RVLDECOMPRESSSTREAM_HPP
