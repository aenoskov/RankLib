
//
// Thread
//
// 15 November 2004 -- tds
//

#ifndef INDRI_THREAD_HPP
#define INDRI_THREAD_HPP

#ifndef WIN32
#include <pthread.h>
#endif

void* pthread_start( void* parameter );
void win32_start( void* parameter );

class Thread {
private:
#ifdef WIN32
  DWORD _id;
  HANDLE _thread;
#else
  pthread_t _thread;
#endif
  void (*_function)( void* data );
  void* _data;

public:
  Thread( void (*function)(void*), void* data );
  void execute();
  void join();
};

#endif // INDRI_THREAD_HPP

