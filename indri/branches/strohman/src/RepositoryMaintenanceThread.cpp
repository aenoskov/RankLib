
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
// maintenance_smoothed_load
//

static float maintenance_smoothed_load( Repository::Load& load ) {
  const float fifteenWeight = 0.2;
  const float fiveWeight = 0.2;
  const float oneWeight = 0.6;

  return load.fifteen * fifteenWeight + load.five * fiveWeight + load.one * oneWeight;
}

//
// maintenance_should_merge
//

static bool maintenance_should_merge( Repository::index_state& state, Repository::Load& documentLoad, Repository::Load& queryLoad ) {
  float addRatio = maintenance_smoothed_load( documentLoad ) / 
                   (maintenance_smoothed_load( queryLoad ) + 1);

  bool couldUseMerge = state->size() >= 2;
  int indexesToMerge = state->size(); 
  
  // extremely heuristic choice for when indexes should be merged:
  //   when we have 100 indexes it makes sense to merge because we'll be out
  //   of file handles soon probably.  Otherwise, we merge if there seem to be
  //   a lot of queries relative to the amount of documents added; and this
  //   is all weighted by the number of indexes we have to merge.

  return couldUseMerge &&
         (indexesToMerge > 100 || (addRatio/50) < indexesToMerge);
}

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
        // if the index is too big, we'd better get to work
        if( _memory < index->memorySize() ) {
          _requests.push( WRITE );
        }

        Repository::Load documentLoad = _repository.documentLoad();
        Repository::Load queryLoad = _repository.queryLoad();

        if( maintenance_should_merge( state, documentLoad, queryLoad ) ) {
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

  _repository._setThrashing( false );

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


