
//
// File
//
// 15 November 2004 -- tds
//

#ifndef INDRI_FILE_HPP
#define INDRI_FILE_HPP

#include "indri/indri-platform.h"
#include "indri/Mutex.hpp"
#include <string>

class File {
private:
#ifdef WIN32
  Mutex _mutex;
  HANDLE _handle;
#else
  int _handle;
#endif

public:
  bool create( const std::string& filename );
  bool open( const std::string& filename );
  bool openRead( const std::string& filename );

  void close();

  size_t read( void* buffer, UINT64 position, size_t length );
  size_t write( const void* buffer, UINT64 position, size_t length );

  UINT64 size();
};

#endif // INDRI_FILE_HPP

