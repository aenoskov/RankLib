
//
// MemoryIndexTermListFileIterator
//
// 24 November 2004 -- tds
//

#include "indri/MemoryIndexTermListFileIterator.hpp"
#include "indri/TermList.hpp"

indri::index::MemoryIndexTermListFileIterator::MemoryIndexTermListFileIterator( std::list<Buffer*>& buffers, std::vector<DocumentData>& data ) :
  _buffers(buffers),
  _data(data)
{
}

void indri::index::MemoryIndexTermListFileIterator::startIteration() {
  _index = 0;
  _list.clear();

  _buffersIterator = _buffers.begin();
  _bufferBase = 0;
}

bool indri::index::MemoryIndexTermListFileIterator::nextEntry() {
  _index++;
  if( _index >= _data.size() )
    return false;

  DocumentData& data = _data[_index];

  // advance buffer iterator if necessary
  while( _bufferBase + (*_buffersIterator)->position() >= data.offset ) {
    _bufferBase += (*_buffersIterator)->position();
    _buffersIterator++;
  }
  assert( _buffersIterator != _buffers.end() );

  // determine the offset into that buffer
  size_t offset = data.offset - _bufferBase;

  // read the term list from there
  _list.read( (*_buffersIterator)->front() + offset, data.byteLength );
  return true;
}

indri::index::TermList* indri::index::MemoryIndexTermListFileIterator::currentEntry() {
  return &_list;
}

bool indri::index::MemoryIndexTermListFileIterator::finished() {
  return _index >= _data.size();
}
