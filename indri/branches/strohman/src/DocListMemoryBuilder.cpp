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
// DocListMemoryBuilder.cpp
//
// tds - 17 December 2003
//

#include "indri/DocListMemoryBuilder.hpp"
#include "lemur/lemur-compat.hpp"
#include "lemur/RVLCompress.hpp"

const int MIN_SIZE = 64;
const int GROW_TIMES = 12;
const size_t PLENTY_OF_SPACE = 15; // docID, count, position: 5 bytes each

//
// DocListMemoryBuilder constructor
//

indri::index::DocListMemoryBuilder::DocListMemoryBuilder( RegionAllocator* allocator ) :
  _list(0),
  _listBegin(0),
  _listEnd(0),
  _documentPointer(0),
  _locationCountPointer(0),
  _lastLocation(0),
  _lastDocument(0),
  _lastTermFrequency(0),
  _termFrequency(0),
  _documentFrequency(0),
  _allocator(allocator)
{
}

//
// DocListMemoryBuilder destructor
//

indri::index::DocListMemoryBuilder::~DocListMemoryBuilder() {
}

//
// _grow
// 

void indri::index::DocListMemoryBuilder::_grow() {
  char* lastList = _list;
  char* lastListBegin = _listBegin;
  char* lastListEnd = _listEnd;
  size_t documentCopyAmount = lastList - _documentPointer;

  // fix data pointer of previous list
  if( lastList != 0 ) {
    if( _locationCountPointer ) {
      _lists.back().data = _documentPointer;
    } else {
      _lists.back().data = lastList;
    }

    assert( _lists.back().data <= _lists.back().capacity );
  }

  // actually add the new list
  unsigned int iterations = std::min<unsigned int>( GROW_TIMES, int(_lists.size()) );
  size_t newSize = (MIN_SIZE << iterations) - 8; // subtract 8 here to give the heap some room for accounting

  _list = (char*) _allocator->allocate( newSize );
  _listBegin = _list;
  _listEnd = _list + newSize;

  _lists.push_back( DocListMemoryBuilderSegment( _listBegin, _listBegin, _listEnd ) );

  // if there's an unterminated document, we have to move it
  if( _locationCountPointer ) {
    memcpy( _list, _documentPointer, documentCopyAmount );
    assert( memset( _documentPointer, 0xcd, lastListEnd - _documentPointer ) );
    // update the _locationCountPointer
    _locationCountPointer = _listBegin + (_locationCountPointer - _documentPointer);
    _list = _listBegin + documentCopyAmount;
    _documentPointer = _listBegin;
  } else {
    _documentPointer = 0;
  }

  assert( !_locationCountPointer || _listBegin < _locationCountPointer );
  assert( !_locationCountPointer || _listEnd > _locationCountPointer );
  assert( !_locationCountPointer || _list > _locationCountPointer );
  assert( (_listEnd - _list) < (MIN_SIZE<<(GROW_TIMES+1)) );
  assert( _listEnd >= _list );
  assert( !_documentPointer || _listBegin <= _documentPointer );
  assert( !_documentPointer || _listEnd > _documentPointer );
}

//
// _terminateDocument
//

void indri::index::DocListMemoryBuilder::_terminateDocument() {
  assert( _locationCountPointer );
  int locations = _termFrequency - _lastTermFrequency;
  int locationsSize = RVLCompress::compressedSize( locations );

  if( locationsSize > 1 ) {
    // have to move everything around to make room, because we need more than
    // one byte to store this length.
    assert( _list > _locationCountPointer );
    assert( _listEnd > _locationCountPointer );
    assert( _listBegin < _locationCountPointer );

    memmove( _locationCountPointer + locationsSize,
             _locationCountPointer + 1,
             _list - _locationCountPointer - 1 );

    _list += locationsSize - 1;
    assert( _list <= _listEnd );
  }

  // we left one byte around for the location count for the common case
  RVLCompress::compress_int( _locationCountPointer, locations );
  _documentFrequency++;
  _lastTermFrequency = _termFrequency;
  _locationCountPointer = 0;
  _lastLocation = 0;
  _documentPointer = 0;

  assert( !_locationCountPointer );
}

//
// _safeAddLocation
//

void indri::index::DocListMemoryBuilder::_safeAddLocation( int documentID, int position ) {
  assert( !_locationCountPointer || _listBegin < _locationCountPointer );
  assert( !_locationCountPointer || _listEnd > _locationCountPointer );
  assert( !_locationCountPointer || _list > _locationCountPointer );

  bool hasPointer = _locationCountPointer ? true : false;
  int lastdoc = _lastDocument;

  if( _lastDocument != documentID ) {
    if( _locationCountPointer )
      _terminateDocument();

    _documentPointer = _list;
    _list = RVLCompress::compress_int( _list, documentID - _lastDocument );
    _locationCountPointer = _list;    
    _list++;
    _lastDocument = documentID;
    _lastLocation = 0;
  }

  _list = RVLCompress::compress_int( _list, position - _lastLocation );
  _lastLocation = position;

  _termFrequency++;

  assert( _locationCountPointer );
  assert( _listBegin < _locationCountPointer );
  assert( _listEnd > _locationCountPointer );
  assert( _list > _locationCountPointer );
  assert( (_listEnd - _list) < (MIN_SIZE<<(GROW_TIMES+1)) );
  assert( _listEnd >= _list );
}

//
// _compressedSize
//

inline size_t indri::index::DocListMemoryBuilder::_compressedSize( int documentID, int position ) {
  size_t size = 0;

  if( _lastDocument != documentID ) {
    size += RVLCompress::compressedSize( documentID - _lastDocument ) + 1;
    size += RVLCompress::compressedSize( _termFrequency - _lastTermFrequency ) - 1;
    size += RVLCompress::compressedSize( position );
  } else {
    size += RVLCompress::compressedSize( position - _lastLocation );
  }

  return size;
}

//
// _growAddLocation
//

void indri::index::DocListMemoryBuilder::_growAddLocation( int documentID, int position, size_t newDataSize ) {
  // have to copy the last document if it's not complete, or if there's not enough room to complete it
  bool terminateSpace = (RVLCompress::compressedSize( _termFrequency - _lastTermFrequency ) - 1) <= _listEnd - _list;

  // by terminating the document now, we save a document copy and a bit of space
  if( _locationCountPointer && terminateSpace && documentID != _lastDocument )
    _terminateDocument();

  // grow the list, adding space for a document if necessary
  _grow();

  assert( newDataSize <= size_t(_listEnd - _list) );
  _safeAddLocation( documentID, position );
}

//
// addLocation
//

void indri::index::DocListMemoryBuilder::addLocation( int documentID, int position ) {
  size_t remaining = size_t(_listEnd - _list);
  assert( _listEnd >= _list );
  assert( remaining < (MIN_SIZE<<(GROW_TIMES+1)) );

  if( remaining >= PLENTY_OF_SPACE ) {
    // common case -- lots of memory; just compress the posting and shove it in
    _safeAddLocation( documentID, position );
  } else {
    size_t size = _compressedSize( documentID, position );

    if( remaining >= size ) {
      _safeAddLocation( documentID, position );
    } else {
      _growAddLocation( documentID, position, size );
    }
  }

  assert( (_listEnd - _list) < (MIN_SIZE<<(GROW_TIMES+1)) );
  assert( _listEnd >= _list );
}

//
// flush
//

void indri::index::DocListMemoryBuilder::flush() {
  if( _locationCountPointer ) {
    // need to terminate document
    bool terminateSpace = (RVLCompress::compressedSize( _termFrequency - _lastTermFrequency ) - 1) <= _listEnd - _list;

    if( !terminateSpace )
      _grow();

    _terminateDocument();
  }

  if( _lists.size() ) {
    _lists.back().data = _list;
    assert( _lists.back().data <= _lists.back().capacity );
  }

  assert( _documentPointer == 0 );
  assert( _locationCountPointer == 0 );

  for( int i=0; i<_lists.size(); i++ ) {
    assert( _lists[i].base <= _lists[i].capacity );
    assert( _lists[i].base <= _lists[i].data );
    assert( _lists[i].data <= _lists[i].capacity );
  }
}

//
// memorySize
//

size_t indri::index::DocListMemoryBuilder::memorySize() const {
  size_t total = 0;

  // the lists follow the sequence MIN_SIZE, MIN_SIZE*2, MIN_SIZE*4, etc.
  // so, total size is (2^(lists+1)-1)*MIN_SIZE.
  int truncLists = std::min<int>( int(_lists.size()), GROW_TIMES );
  total = ((1 << (truncLists+1)) - 1) * MIN_SIZE;

  // each remaining list is max size
  int remainingLists = std::max<int>( (int)_lists.size() - GROW_TIMES, 0 );
  total += (MIN_SIZE << GROW_TIMES) * remainingLists;

  return total;
}

//
// documentFrequency
//

int indri::index::DocListMemoryBuilder::documentFrequency() const {
  return _documentFrequency;
}

//
// termFrequency
//

int indri::index::DocListMemoryBuilder::termFrequency() const {
  return _termFrequency;
}

//
// getIterator
//

indri::index::DocListMemoryBuilderIterator* indri::index::DocListMemoryBuilder::getIterator() {
  return new DocListMemoryBuilderIterator( *this );
}

