
//
// DocListMemoryBuilderIterator
//
// 26 November 2004 -- tds
//

#include "indri/DocListMemoryBuilder.hpp"

//
// startIteration
//

void indri::index::DocListMemoryBuilderIterator::startIteration() {
  _current = _lists->begin();

  if( _current != _lists->end() ) {
    _list = _current->base;
    _listEnd = _current->data;
  } else {
    _list = 0;
    _listEnd = 0;
  }
  
  _data.document = 0;
  _data.positions.clear();

  nextEntry();
}

//
// reset
//

void indri::index::DocListMemoryBuilderIterator::reset( DocListMemoryBuilder& builder ) {
  builder.flush();
  reset( builder._lists );
}

//
// reset
//

void indri::index::DocListMemoryBuilderIterator::reset( const greedy_vector< DocListMemoryBuilderSegment, 4 >& lists ) {
  _lists = &lists;
  _current = _lists->begin();
  
  if( _current != _lists->end() ) {
    _list = _current->base;
    _listEnd = _current->data;
  } else {
    _list = 0;
    _listEnd = 0;
  }
  
  _data.document = 0;
  _data.positions.clear();
  _finished = false;

  nextEntry();
}

//
// DocListMemoryBuilderIterator constructor
//

indri::index::DocListMemoryBuilderIterator::DocListMemoryBuilderIterator() {
}

//
// DocListMemoryBuilderIterator constructor
//

indri::index::DocListMemoryBuilderIterator::DocListMemoryBuilderIterator( class DocListMemoryBuilder& builder )
{
  reset( builder );
}

//
// nextEntry
//

bool indri::index::DocListMemoryBuilderIterator::nextEntry( int documentID ) {
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
      
bool indri::index::DocListMemoryBuilderIterator::nextEntry() {
  if( _list < _listEnd ) {
    int deltaDocument;
    int extents;
    
    _list = RVLCompress::decompress_int( _list, deltaDocument );
    _data.document += deltaDocument;
    _data.positions.clear();

    _list = RVLCompress::decompress_int( _list, extents );

    int deltaPosition;
    int lastPosition = 0;

    for( int i=0; i<extents; i++ ) {
      _list = RVLCompress::decompress_int( _list, deltaPosition );
      _data.positions.push_back( deltaPosition + lastPosition );
      lastPosition += deltaPosition;
    }
  } else {    
    assert( _list == _listEnd );

    // no data left, go to the next segment
    if( _current != _lists->end() )
      _current++;
    
    if( _current != _lists->end() ) {
      _list = _current->base;
      _listEnd = _current->data;
      return nextEntry();
    }

    // no more list segments
    _finished = true;
    return false;
  }

  return true;
}
      
//
// currentEntry
//

indri::index::DocListIterator::DocumentData* indri::index::DocListMemoryBuilderIterator::currentEntry() {
  if( !finished() )
    return &_data;

  return 0;
}

//
// finished
//

bool indri::index::DocListMemoryBuilderIterator::finished() {
  return _finished;
}

//
// topDocuments
//

greedy_vector<indri::index::DocListIterator::TopDocument>& indri::index::DocListMemoryBuilderIterator::topDocuments() {
  return _emptyTopDocuments;
}


