
//
// ConditionVariable
//
// 15 November 2004 -- tds
//
// Implementation based on code from the ACE library (Schmidt)
//

#ifndef INDRI_CONDITIONVARIABLE_HPP
#define INDRI_CONDITIONVARIABLE_HPP

#ifndef WIN32
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#endif


#include "indri/indri-platform.h"
#include "Mutex.hpp"

class ConditionVariable {
private:
#ifdef WIN32
  HANDLE _event;
  int _waiters;
#else
  pthread_cond_t _condition;
#endif

public:
  ConditionVariable() {
    #ifdef WIN32
        _event = ::CreateEvent( NULL, FALSE, FALSE, NULL );
        _waiters = 0;
    #else
        pthread_cond_init( &_condition, NULL );
    #endif
  }

  ~ConditionVariable() {
    #ifdef WIN32
      ::CloseHandle( _event );
    #else
      pthread_cond_destroy( &_condition );
    #endif
  }

  void wait( Mutex& mutex ) {
#ifdef WIN32
    HRESULT result = ::SignalObjectAndWait( mutex._mutex, _event, INFINITE, FALSE );
    assert( SUCCEEDED(result) );
    mutex.lock();
#else
    pthread_cond_wait( &_condition, &mutex._mutex );
#endif
  }

  bool wait( Mutex& mutex, UINT64 microseconds ) {
#ifdef WIN32
    HRESULT result = ::SignalObjectAndWait( mutex._mutex, _event, DWORD(microseconds/1000), FALSE );
    assert( SUCCEEDED(result) );
    mutex.lock();
    return result != WAIT_TIMEOUT;
#else
    mutex.unlock();
    struct timeval tv;
    gettimeofday( &tv, 0 );

    microseconds += tv.tv_sec * 10000000;
    microseconds += tv.tv_usec;

    struct timespec ts;
    ts.tv_sec = microseconds / 1000000;
    ts.tv_nsec = (microseconds % 1000000) * 1000;

    int result = pthread_cond_timedwait( &_condition, &mutex._mutex, &ts );
    mutex.lock();

    return result != ETIMEDOUT;
#endif
  }


  void notifyOne( ) {
#ifdef WIN32
    ::SetEvent( _event );
#else
  pthread_cond_signal( &_condition );
#endif
  }

  void notifyAll( ) {
#ifdef WIN32
    ::SetEvent( _event );
#else
    pthread_cond_broadcast( &_condition );
#endif
  }
};

#endif // INDRI_CONDITIONVARIBLE_HPP

