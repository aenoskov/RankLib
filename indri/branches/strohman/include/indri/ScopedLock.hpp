
//
// ScopedLock
//
// 15 November 2004 -- tds
//

#ifndef INDRI_SCOPEDLOCK_HPP
#define INDRI_SCOPEDLOCK_HPP

class ScopedLock {
private:
  Mutex* _mutex;

public:
  ScopedLock( Mutex& mutex ) {
    _mutex = &mutex;
    _mutex->lock();
  }

  ~ScopedLock() {
    if( _mutex )
      _mutex->unlock();
  }

  void unlock() {
    _mutex->unlock();
    _mutex = 0;
  }
};

#endif // INDRI_SCOPEDLOCK_HPP

