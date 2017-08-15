#include "elementsmanager.h"

namespace Memory
{

ElementsManager::ElementsManager()
{

}

ElementsManager::~ElementsManager()
{
  for(auto it = map_rwl_.begin(); it != map_rwl_.end(); ++it) {
    delete (*it);
  }
}

void ElementsManager::lockForRead(TME *me)
{
  if(!map_rwl_.contains(me))
    map_rwl_[me] = new QReadWriteLock;
  map_rwl_[me]->lockForRead();
}

void ElementsManager::lockForWrite(TME *me)
{
  if(!map_rwl_.contains(me))
    map_rwl_[me] = new QReadWriteLock;
  map_rwl_[me]->lockForWrite();
}

void ElementsManager::unlock(TME *me)
{
  if(map_rwl_.contains(me))
    map_rwl_[me]->unlock();
}

}
