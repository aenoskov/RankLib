
//
// DocExtentListMemoryBuilderIterator
//
// 26 November 2004 -- tds
//

#include "indri/DocExtentListMemoryBuilder.hpp"

//
// startIteration
//

void indri::index::DocExtentListMemoryBuilderIterator::startIteration() {
}

//
// reset
//

void indri::index::DocExtentListMemoryBuilderIterator::reset( const DocExtentListMemoryBuilder& builder ) {
  reset( builder._lists, builder._numeric );
}

//
// reset
//

void indri::index::DocExtentListMemoryBuilderIterator::reset( const greedy_vector< std::pair<char*,char*>, 4 >& lists, bool numeric ) {
  _lists = &lists;
  _numeric = numeric;
  
  _current = _lists->begin();
  
  if( _current != _lists->end() ) {
    _list = _current->first;
    _listEnd = _current->second;
  } else {
    _list = 0;
    _listEnd = 0;
  }
  
  _data.document = 0;
  _data.numbers.clear();
  _data.extents.clear();

  nextEntry();
}

//
// DocExtentListMemoryBuilderIterator constructor
//

indri::index::DocExtentListMemoryBuilderIterator::DocExtentListMemoryBuilderIterator( const class DocExtentListMemoryBuilder& builder )
{
  reset( builder._lists, builder._numeric );
}

//
// finished
//

bool indri::index::DocExtentListMemoryBuilderIterator::finished() {
  return _current == _lists->end() && _list == _listEnd;
}

//
// nextEntry
//

bool indri::index::DocExtentListMemoryBuilderIterator::nextEntry( int documentID ) {
  do {
    if( _data.document >= documentID )
      return true;
  }
  while( nextEntry() );
  
  return false;
}

//
// nextEntry
//
      
bool indri::index::DocExtentListMemoryBuilderIterator::nextEntry() {
  if( _list < _listEnd ) {
    int deltaDocument;
    int extents;
    
    _list = RVLCompress::decompress_int( _list, deltaDocument );
    _data.document += deltaDocument;
    _data.extents.clear();
    _data.numbers.clear();

    _list = RVLCompress::decompress_int( _list, extents );

    int deltaPosition;
    Extent extent;
    extent.begin = 0;
    extent.end = 0;

    for( int i=0; i<extents; i++ ) {
      _list = RVLCompress::decompress_int( _list, deltaPosition );
      extent.begin = extent.end + deltaPosition;
      _list = RVLCompress::decompress_int( _list, deltaPosition );
      extent.end = extent.begin + deltaPosition;

      _data.extents.push_back( extent );

      if( _numeric ) {
        INT64 number;
        _list = RVLCompress::decompress_signed_longlong( _list, number );
        _data.numbers.push_back( number );
      }
    }
  } else {    
    assert( _list == _listEnd );

    // no data left, go to the next segment
    if( _current != _lists->end() )
      _current++;
    
    if( _current != _lists->end() ) {
      _list = _current->first;
      _listEnd = _current->second;
      return nextEntry();
    }

    // no more list segments
    return false;
  }

  return true;
}
      
//
// currentEntry
//

indri::index::DocExtentListIterator::DocumentExtentData* indri::index::DocExtentListMemoryBuilderIterator::currentEntry() {
  return &_data;
}
