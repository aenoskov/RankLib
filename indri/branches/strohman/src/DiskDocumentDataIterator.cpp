
//
// DiskDocumentDataIterator
//
// 21 January 2004 -- tds
//

#include "indri/DiskDocumentDataIterator.hpp"

//
// DiskDocumentDataIterator constructor
//

indri::index::DiskDocumentDataIterator::DiskDocumentDataIterator( File& documentDataFile ) :
  _documentDataFile(documentDataFile),
  _readBuffer( new SequentialReadBuffer( documentDataFile, 1024*1024 ) )
{
}

//
// DiskDocumentDataIterator destructor
//

indri::index::DiskDocumentDataIterator::~DiskDocumentDataIterator() {
  delete _readBuffer;
}

//
// startIteration
//

void indri::index::DiskDocumentDataIterator::startIteration() {
  _fileSize = _documentDataFile.size();
  _readBuffer->seek( 0 );
  _readBuffer->read( &_documentData, sizeof(DocumentData) );
}

//
// nextEntry
//

bool indri::index::DiskDocumentDataIterator::nextEntry() {
  if( !finished() )
    _readBuffer->read( &_documentData, sizeof(DocumentData) );

  return !finished();
}

//
// currentEntry
//

const indri::index::DocumentData* indri::index::DiskDocumentDataIterator::currentEntry() {
  if( !finished() )
    return &_documentData;

  return 0;
}

//
// finished
//

bool indri::index::DiskDocumentDataIterator::finished() {
  return _readBuffer->position() < _fileSize;
}


