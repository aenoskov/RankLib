
//
// RepositoryLoadThread
//
// 31 January 2005 -- tds
//

#include "indri/RepositoryLoadThread.hpp"
#include "indri/Repository.hpp"

const int FIVE_SECONDS = 5*1000*1000;

//
// constructor
//

RepositoryLoadThread::RepositoryLoadThread( Repository& repository ) :
  UtilityThread(),
  _repository( repository )
{
}

//
// initialize
//

UINT64 RepositoryLoadThread::initialize() {
  return FIVE_SECONDS;
}

//
// deinitialize
//

void RepositoryLoadThread::deinitialize() {
  // do nothing
}

//
// work
//

UINT64 RepositoryLoadThread::work() {
  _repository._incrementLoad();
  return FIVE_SECONDS;
}

//
// hasWork
//

bool RepositoryLoadThread::hasWork() {
  return false;
}



