
//
// RepositoryMaintenanceThread
//
// 31 January 2005 -- tds
//

#include "indri/RepositoryMaintenanceThread.hpp"
#include "indri/Repository.hpp"
#include "indri/ScopedLock.hpp"

const UINT64 TIME_DELAY = 5*1000*1000;
const UINT64 ACTIVE_DELAY = 1*1000*1000;

//
// constructor
//

RepositoryMaintenanceThread::RepositoryMaintenanceThread( Repository& repository, UINT64 memory ) :
  UtilityThread(),
  _repository( repository ),
  _memory( memory )
{
}

//
// initialize
//

UINT64 RepositoryMaintenanceThread::initialize() {
  return TIME_DELAY;
}

//
// deinitialize
//

void RepositoryMaintenanceThread::deinitialize() {
  // do nothing
}

//
// work
//

UINT64 RepositoryMaintenanceThread::work() {
  // fetch current index state
  bool write = false;
  bool merge = false;

  {
    // lock the request queue
    ScopedLock l( _requestLock );

    // if nobody has any requests, check to see if we should be working
    if( ! _requests.size() ) {
      // get the memory size of the active memory index
      Repository::index_state state = _repository.indexes();
      indri::index::MemoryIndex* index = dynamic_cast<indri::index::MemoryIndex*>(state->back());

      if( index ) {
        if( _memory < index->memorySize() ) {
          _requests.push( WRITE );
        }

        if( state->size() > 2 ) {
          _requests.push( MERGE );
        }
      }
    }

    // now, figure out what needs to be done
    while( _requests.size() ) {
      switch( _requests.front() ) {
      case MERGE:
        merge = true;
        break;

      case WRITE:
        write = true;
        break;
      }

      _requests.pop();
    }
  
    // unlock request queue
  }

  if( merge ) {
    _repository._merge();
  } else if( write ) {
    _repository._write();
  }

  if( merge || write )
    return ACTIVE_DELAY;
  else
    return TIME_DELAY;
}

//
// hasWork
//

bool RepositoryMaintenanceThread::hasWork() {
  ScopedLock l( _requestLock );
  return _requests.size() > 0;
}

//
// write
//

void RepositoryMaintenanceThread::write() {
  ScopedLock l( _requestLock );
  _requests.push( WRITE );
}

//
// merge
//

void RepositoryMaintenanceThread::merge() {
  ScopedLock l( _requestLock );
  _requests.push( MERGE );
}


