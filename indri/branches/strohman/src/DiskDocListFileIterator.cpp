
//
// DiskDocListFileIterator 
//
// 13 December 2004 -- tds
//

#include "indri/DiskDocListFileIterator.hpp"

//
// _readEntry
//

void indri::index::DiskDocListFileIterator::_readEntry() {
  int headerLength;
  UINT64 startPosition;

  // figure out where we are, and how much header data needs to be read
  startPosition = _file->position();
  _file->read( &headerLength, sizeof headerLength );

  // read in the header
  _header.clear();

  _file->read( _header.write( headerLength ), headerLength );
  RVLDecompressStream stream( _header.front(), _header.position() );

  // header is RVLCompressed with the term first, followed by a termData structure
  stream >> _term;
  ::termdata_decompress( stream, _termData, _fieldCount );
  _termData->term = _term;

  // set up the iterator to find the appropriate data
  _iterator.setStartOffset( startPosition );

  _docListData.iterator = &_iterator;
  _docListData.termData = _termData;
}

//
// DiskDocListFileIterator
//

indri::index::DiskDocListFileIterator::DiskDocListFileIterator( File& docListFile, int fieldCount ) : 
  _file( new SequentialReadBuffer( docListFile ) ),
  _fileLength( docListFile.size() ),
  _fieldCount( fieldCount ),
  _iterator( _file, 0 )
{
}

//
// startIteration
//

void indri::index::DiskDocListFileIterator::startIteration() {
  if( !finished() ) {
    _readEntry();
  }
}

//
// nextEntry
//

bool indri::index::DiskDocListFileIterator::nextEntry() {
  assert( !finished() );
  _readEntry();
  return true;
}

//
// currentEntry
//

indri::index::DocListFileIterator::DocListData* indri::index::DiskDocListFileIterator::currentEntry() {
  return &_docListData;
}

//
// currentEntry
//

const indri::index::DocListFileIterator::DocListData* indri::index::DiskDocListFileIterator::currentEntry() const {
  return &_docListData;
}

//
// finished
//

bool indri::index::DiskDocListFileIterator::finished() const {
  return _fileLength > _file->position();
}

