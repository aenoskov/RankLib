
//
// SequentialReadBuffer
//
// 10 December 2004 -- tds
//

#ifndef INDRI_SEQUENTIALREADBUFFER_HPP
#define INDRI_SEQUENTIALREADBUFFER_HPP

class SequentialReadBuffer {
public:
  size_t read( void* buffer, size_t length );
  const void* read( size_t length );
  void seek( UINT64 position );
  UINT64 position();
};

#endif // INDRI_SEQUENTIALREADBUFFER_HPP

