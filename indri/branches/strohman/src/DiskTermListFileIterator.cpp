
//
// DiskTermListFileIterator
//
// 26 January 2005 -- tds 
//

#include "indri/DiskTermListFileIterator.hpp"

//
// DiskTermListFileIterator
//

indri::index::DiskTermListFileIterator::DiskTermListFileIterator( File& termListFile ) :
  _termListFile(termListFile),
  _buffer(_termListFile, 1024*1024),
  _fileSize(termListFile.size())
{
}

//
// startIteration
//

void indri::index::DiskTermListFileIterator::startIteration() {
  nextEntry();
}

//
// currentEntry
//

indri::index::TermList* indri::index::DiskTermListFileIterator::currentEntry() {
  if( !finished() )
    return &_termList;

  return 0;
}

//
// nextEntry
//

bool indri::index::DiskTermListFileIterator::nextEntry() {
  if( !finished() ) {
    UINT32 length;
    _buffer.read( &length, sizeof(UINT32) );
    _termList.read( (const char*) _buffer.read( length ), length );
  }

  return !finished();
}

//
// finished
//

bool indri::index::DiskTermListFileIterator::finished() {
  return _buffer.position() >= _fileSize;
}
