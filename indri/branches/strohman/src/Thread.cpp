
//
// Thread
//
// 15 November 2004 -- tds
//

#include "Thread.hpp"

Thread::Thread( void (*function)(void*), void* data ) {
  _function = function;
  _data = data;

  #ifdef WIN32
    _thread = _beginthreadex( 0, 0, win32_start, this, 0, &_id );
  #else
    pthread_create( &_thread, 0, pthread_start, this );
  #endif
}

void Thread::execute() {
  _function( _data );
}

void Thread::join() {
  #ifdef WIN32
    WaitForSingleObject( _thread, INFINITE );
    CloseHandle( _thread );
  #else
    pthread_join( _thread, 0 );
  #endif
}

void* pthread_start( void* parameter ) {
  Thread* t = (Thread*) parameter;
  t->execute();
  return 0;
}

void win32_start( void* parameter ) {
  Thread* t = (Thread*) parameter;
  t->execute();
}

