/*==========================================================================
  Copyright (c) 2004 University of Massachusetts.  All Rights Reserved.

  Use of the Lemur Toolkit for Language Modeling and Information Retrieval
  is subject to the terms of the software license set forth in the LICENSE
  file included with this software, and also available at
  http://www.cs.cmu.edu/~lemur/license.html 
  as well as the conditions below.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in
  the documentation and/or other materials provided with the
  distribution.

  3. The names "Indri", "Center for Intelligent Information Retrieval", 
  "CIIR", and "University of Massachusetts" must not be used to
  endorse or promote products derived from this software without
  prior written permission. To obtain permission, contact
  indri-info@ciir.cs.umass.edu.

  4. Products derived from this software may not be called "Indri" nor 
  may "Indri" appear in their names without prior written permission of 
  the University of Massachusetts. To obtain permission, contact 
  indri-info@ciir.cs.umass.edu.

  THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY OF MASSACHUSETTS AND OTHER
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
  FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
  THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.
  ==========================================================================
*/


//
// RVLCompressStream
//
// 9 February 2004 -- tds
//

#ifndef INDRI_RVLCOMPRESSSTREAM_HPP
#define INDRI_RVLCOMPRESSSTREAM_HPP

#include "lemur/RVLCompress.hpp"
#include "indri/Buffer.hpp"

/*! Provide RVL compression on a stream.
 */
class RVLCompressStream {
private:
  Buffer& _buffer;

public:
  /// Initialize
  /// @param buffer the buffer to use for compressing
  /// @param size the size of buffer
  RVLCompressStream( Buffer& buffer ) :
    _buffer(buffer)
  {
  }

  /// @return the buffer
  const char* data() const {
    return _buffer.front();
  }
  /// @return the size of the data in the buffer
  size_t dataSize() const {
    return _buffer.position();
  }

  /// Compress an int into the buffer
  /// @param value the value to compress
  RVLCompressStream& operator<< ( int value ) {
    char* writePosition = _buffer.write(5);
    char* endPosition = RVLCompress::compress_int( writePosition, value );
    _buffer.unwrite( 5 - (endPosition - writePosition) );
    return *this;
  }

  /// Compress an unsigned int into the buffer
  /// @param value the value to compress
  RVLCompressStream& operator<< ( unsigned int value ) {
    char* writePosition = _buffer.write(5);
    char* endPosition = RVLCompress::compress_int( writePosition, value );
    _buffer.unwrite( 5 - (endPosition - writePosition) );
    return *this;
  }

  /// Compress an INT64 into the buffer
  /// @param value the value to compress
  RVLCompressStream& operator<< ( INT64 value ) {
    char* writePosition = _buffer.write(10);
    char* endPosition = RVLCompress::compress_longlong( writePosition, value );
    _buffer.unwrite( 10 - (endPosition - writePosition) );
    return *this;
  }

  /// Compress a float into the buffer
  /// @param value the value to compress
  RVLCompressStream& operator << ( float value ) {
    // can't compress a float, unfortunately
    memcpy( _buffer.write(sizeof(float)), &value, sizeof value );
    return *this;
  }

  /// Compress a string into the buffer
  RVLCompressStream& operator << ( const char* value ) {
    unsigned int length = (unsigned int) strlen( value );
    (*this) << length;
    memcpy( _buffer.write(length), value, length );
    return *this;
  }
};

#endif // INDRI_RVLCOMPRESSSTREAM_HPP

