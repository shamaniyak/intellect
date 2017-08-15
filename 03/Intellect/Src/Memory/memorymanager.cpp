#include "memorymanager.h"
#include "tmemory.h"

MemoryManager::MemoryManager(QObject *parent) : QObject(parent)
{

}

MemoryWrapper *MemoryManager::Add(const QString &name)
{
  if(!map_mw_.contains(name)) {
    //TMemory *M = new TMemory(name, this);
    //map_mems_[name] = M;
    map_mw_[name] = new MemoryWrapper(this);
  }

  //return map_mems_[name];
  return map_mw_[name];
}

MemoryWrapper *MemoryManager::Get(const QString &name)
{
  return Add(name);
}

void MemoryManager::Del(const QString &name)
{
  if(map_mw_.contains(name)) {
    //delete map_mems_[name];
    //map_mems_.remove(name);
    delete map_mw_[name];
    map_mw_.remove(name);
  }
}
