
//
// DiskDocListIterator
//
// 10 December 2004 -- tds
//

#include "indri/DiskDocListIterator.hpp"
#include "lemur/RVLCompress.hpp"

//
// ---------------------
// Inverted list format:
// ---------------------
//      int (4b)    headerLength
//         RVLCompressed section (size is headerLength)
//            termString
//            termData (indri::index::TermData structure)
//      byte (1b)   controlByte   (0x01 = hasSkips, 0x02 = hasTopdocs)
//      topdocsCount (4b)  (if hasTopdocs)
//         for each topdoc:
//         docID (4b)
//         count (4b)
//         length (4b)
//      raw inverted list data:
//        skip: (if hasSkips)
//          (4b) document
//          (4b) length
//        for each in a batch of documents:
//          RVLCompressed:
//            delta document ID (delta encoded by batch)
//            position count
//            delta encoded positions
//
// ----------------------------
// More explanation about skips:
// ----------------------------
//
// A skip is a 8 byte structure that helps the query processor skip
// through the inverted list efficiently.  The first field, document,
// is the ID of some document, and the length parameter indicates the
// number bytes that need to be skipped to find that document.
// The number of bytes is measured from the end of the skip structure.
//
// At the beginning of the last chunk of documents, the skip will be {-1,-1}.
//

//
// DiskDocListIterator constructor
//

indri::index::DiskDocListIterator::DiskDocListIterator( SequentialReadBuffer* buffer, UINT64 startOffset )
  :
  _file(buffer),
  _startOffset(startOffset)
{
}

//
// DiskDocListIterator destructor
//

indri::index::DiskDocListIterator::~DiskDocListIterator() {
  delete _file;
}

//
// setEndpoints
//

void indri::index::DiskDocListIterator::setStartOffset( UINT64 startOffset ) {
  _startOffset = startOffset;
  _topdocs.clear();
  _file->seek( _startOffset );
}

//
// topDocuments
//

const greedy_vector<indri::index::DocListIterator::TopDocument>& indri::index::DiskDocListIterator::topDocuments() {
  return _topdocs;
}

//
// startIteration
//

void indri::index::DiskDocListIterator::startIteration() {
  // seek to the start:
  _file->seek( _startOffset );

  // read the header length
  UINT32 headerLength;
  _file->read( &headerLength, sizeof(UINT32) );

  // skip over the header
  _file->seek( _file->position() + headerLength );

  // read the control byte
  UINT8 control;
  _file->read( &control, sizeof(UINT8) );

  _hasTopdocs = (control & 0x01) ? true : false;
  
  // clear out all the internal data
  _data.document = 0;
  _data.positions.clear();
  _skipDocument = -1;
  _list = _listEnd = 0;

  // read in the topdocs information
  _readTopdocs();

  // read in the first entry
  _readSkip();
  _readEntry();
}

//
// nextEntry
//

bool indri::index::DiskDocListIterator::nextEntry() {
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

bool indri::index::DiskDocListIterator::nextEntry( int documentID ) {
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

indri::index::DiskDocListIterator::DocumentData* indri::index::DiskDocListIterator::currentEntry() {
  return &_data;
}

//
// finished
//

bool indri::index::DiskDocListIterator::finished() {
  return _list != _listEnd || _skipDocument > 0;
}

//
// _readTopdocs
//

void indri::index::DiskDocListIterator::_readTopdocs() {
  if( !_hasTopdocs ) return;

  int topdocsCount;
  _topdocs.clear();
  _file->read( &topdocsCount, sizeof(int) );

  for( int i=0; i<topdocsCount; i++ ) {
    int documentID, count, length;

    _file->read( &documentID, sizeof(int) );
    _file->read( &count, sizeof(int) );
    _file->read( &length, sizeof(int) );

    _topdocs.push_back( TopDocument( documentID, count, length ) );
  }
}

//
// _readSkip
//

void indri::index::DiskDocListIterator::_readSkip() {
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

void indri::index::DiskDocListIterator::_readEntry() {
  _data.positions.clear();
  
  int deltaDocument;
  _list = RVLCompress::decompress_int( _list, deltaDocument );
  _data.document += deltaDocument;

  int numPositions;
  _list = RVLCompress::decompress_int( _list, numPositions );

  int lastPosition = 0;
  int deltaPosition;
  
  for( int i=0; i<numPositions; i++ ) {
    _list = RVLCompress::decompress_int( _list, deltaPosition );
    _data.positions.push_back( deltaPosition + lastPosition );
    lastPosition += deltaPosition;
  }
}
