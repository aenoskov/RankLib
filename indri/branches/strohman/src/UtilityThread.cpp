
//
// UtilityThread
//
// 31 January 2005 -- tds
//

#include "indri/UtilityThread.hpp"
#include "indri/ScopedLock.hpp"

//
// utility_thread_run
//

void utility_thread_run( void* pointer ) {
  ( (UtilityThread*) pointer )->run();
}

//
// UtilityThread
//

UtilityThread::UtilityThread() :
  _thread(0)
{
}

//
// run
//

void UtilityThread::run() {
  ScopedLock lock( _lock );

  UINT64 waitTime = initialize();

  while( _runThread ) {
    bool noTimeout = _quit.wait( _lock, waitTime );

    if( noTimeout )
      continue; // interrupted

    waitTime = work();
  }

  while( hasWork() )
    work();

  deinitialize();
}

//
// start
//

void UtilityThread::start() {
  if( _thread )
    return;

  _thread = new Thread( utility_thread_run, this ); 
}

//
// signal
//

void UtilityThread::signal() {
  _runThread = false;
  _quit.notifyAll();
}

//
// join
//

void UtilityThread::join() {
  signal();

  if( _thread ) {
    _thread->join();
    delete _thread;
    _thread = 0;
  }
}

