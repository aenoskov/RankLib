
//
// DiskDocExtentListIterator
//
// 13 December 2004 -- tds
//

#include "indri/indri-platform.h"
#include "indri/DiskDocExtentListIterator.hpp"
#include "lemur/RVLCompress.hpp"

//
// DiskDocExtentListIterator constructor
//

indri::index::DiskDocExtentListIterator::DiskDocExtentListIterator( SequentialReadBuffer* buffer, UINT64 startOffset )
  :
  _file(buffer),
  _startOffset(startOffset)
{
}

//
// DiskDocExtentListIterator destructor
//

indri::index::DiskDocExtentListIterator::~DiskDocExtentListIterator() {
  delete _file;
}

//
// setEndpoints
//

void indri::index::DiskDocExtentListIterator::setStartOffset( UINT64 startOffset ) {
  _startOffset = startOffset;
  _file->seek( _startOffset );
}

//
// startIteration
//

void indri::index::DiskDocExtentListIterator::startIteration() {
  // seek to the start:
  _file->seek( _startOffset );

  // read the control byte
  UINT8 control;
  _file->read( &control, sizeof(UINT8) );

  _numeric = (control & 0x02) ? true : false;
  
  // clear out all the internal data
  _data.document = 0;
  _data.extents.clear();
  _data.numbers.clear();
  _skipDocument = -1;
  _list = _listEnd = 0;

  // read in the first entry
  _readSkip();
  _readEntry();
}

//
// nextEntry
//

bool indri::index::DiskDocExtentListIterator::nextEntry() {
  if( _list == _listEnd ) {
    if( _skipDocument > 0 ) {
      // need to read the next segment of this list
      _readSkip();
      _readEntry();
      return true;
    } else {
      // all done
      return false;
    }
  }

  _readEntry();
  return true;
}

//
// nextEntry
//

bool indri::index::DiskDocExtentListIterator::nextEntry( int documentID ) {
  // skip ahead as much as possible
  while( _skipDocument > 0 && _skipDocument <= documentID ) {
    _readSkip();
  }

  // now, read entries until we find one that's good
  while( _data.document < documentID && _list != _listEnd ) {
    _readEntry();
  }

  return _list != _listEnd;
}

//
// currentEntry
//

indri::index::DiskDocExtentListIterator::DocumentExtentData* indri::index::DiskDocExtentListIterator::currentEntry() {
  return &_data;
}

//
// finished
//

bool indri::index::DiskDocExtentListIterator::finished() const {
  return _list != _listEnd || _skipDocument > 0;
}

//
// _readSkip
//

void indri::index::DiskDocExtentListIterator::_readSkip() {
  int skipLength; 

  _file->read( &_skipDocument, sizeof(int) );
  _file->read( &skipLength, sizeof(int) );

  _list = static_cast<const char*>(_file->read( skipLength ));
  _listEnd = _list + skipLength;
  _data.document = 0;
}

//
// _readEntry
//

void indri::index::DiskDocExtentListIterator::_readEntry() {
  _data.extents.clear();
  
  int deltaDocument;
  _list = RVLCompress::decompress_int( _list, deltaDocument );

  _data.document += deltaDocument;

  int numPositions;
  _list = RVLCompress::decompress_int( _list, numPositions );

  int lastPosition = 0;
  INT64 number;
  
  for( int i=0; i<numPositions; i++ ) {
    Extent extent;

    _list = RVLCompress::decompress_int( _list, extent.begin );
    _list = RVLCompress::decompress_int( _list, extent.end );

    // delta-dencode
    extent.begin += lastPosition;
    extent.end += extent.begin;
    lastPosition = extent.end;

    _data.extents.push_back( extent );

    if( _numeric ) {
      _list = RVLCompress::decompress_signed_longlong( _list, number );
      _data.numbers.push_back( number );
    }
  }
}
