
//
// Mutex
//
// 15 November 2004 -- tds
//

#ifndef INDRI_MUTEX_HPP
#define INDRI_MUTEX_HPP

#include <assert.h>

#ifndef WIN32
#include <pthread.h>
#endif

class Mutex {
  friend class ConditionVariable;

private:
#ifdef WIN32
  CRITICAL_SECTION _cs;
#else // POSIX
  pthread_mutex_t _mutex;
#endif

public:
  Mutex() {
#ifdef WIN32
    ::InitializeCriticalSection( &_cs );
#else
    pthread_mutex_init( &_mutex, NULL );
#endif
  }

  ~Mutex() {
#ifdef WIN32
    ::DestroyCriticalSection( &_cs );
#else
  pthread_mutex_destroy( &_mutex );
#endif
  }

  void lock() {
#ifdef WIN32
    ::EnterCriticalSection( &_cs );
#else
    int result = pthread_mutex_lock( &_mutex );
    assert( result == 0 );
#endif
  }

  bool tryLock() {
#ifdef WIN32
    BOOL result = ::TryEnterCriticalSection( &_cs );
    return result ? true : false;
#else
    return pthread_mutex_trylock( &_mutex ) == 0 ? true : false;
#endif
  }

  void unlock() {
#ifdef WIN32
    ::LeaveCriticalSection( &_cs );
#else
    int result = pthread_mutex_unlock( &_mutex );
    assert( result == 0 );
#endif
  }
};

#endif // INDRI_MUTEX_HPP

