
//
// DiskFrequentVocabularyIterator
//
// 19 January 2005 -- tds
//

#include "indri/DiskFrequentVocabularyIterator.hpp"
#include "indri/DiskTermData.hpp"

//
// DiskFrequentVocabularyIterator constructor
//

indri::index::DiskFrequentVocabularyIterator::DiskFrequentVocabularyIterator( File& frequentTermsData, int fieldCount ) :
  _file(frequentTermsData),
  _fieldCount(fieldCount),
  _stream(0, 0)
{
}

//
// startIteration
//

void indri::index::DiskFrequentVocabularyIterator::startIteration() {
  if( _buffer.size() == 0 ) {
    UINT64 length = _file.size();
    _file.read( _buffer.write( length ), 0, length );
  }

  _stream.setBuffer( _buffer.front(), _buffer.size() );
}

//
// nextEntry
//

bool indri::index::DiskFrequentVocabularyIterator::nextEntry() {
  _data = ::disktermdata_decompress( _stream, _dataBuffer, _fieldCount, indri::index::DiskTermData::WithOffsets |
                                                                        indri::index::DiskTermData::WithTermID | 
                                                                        indri::index::DiskTermData::WithString );

  return !finished();
}

//
// finished
//

bool indri::index::DiskFrequentVocabularyIterator::finished() {
  return _current != _buffer.front() + _buffer.position();
}

//
// currentEntry
//

indri::index::DiskTermData* indri::index::DiskFrequentVocabularyIterator::currentEntry() {
  return _data;
}
