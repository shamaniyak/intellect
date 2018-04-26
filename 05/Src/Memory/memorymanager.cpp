#include "memorymanager.h"
#include "tmemory.h"
#include "qmemorymodel.h"

//  TMemoryManager

MemoryWrapper *MemoryManager::globalMemory_ = nullptr;
int MemoryManager::countLinksToGlobalMemory_= 0;

MemoryManager::MemoryManager(QObject *parent) : QObject(parent)
{
  if(!globalMemory_) {
    globalMemory_ = new QMemoryModel();
    globalMemory_->setObjectName("globalMemory");
  }
  ++countLinksToGlobalMemory_;
}

MemoryManager::~MemoryManager()
{
  --countLinksToGlobalMemory_;
  if(countLinksToGlobalMemory_ == 0) {
    delete globalMemory_;
    globalMemory_ = nullptr;
  }
}

MemoryWrapper *MemoryManager::Add(const QString &name)
{
  auto mem = this->findChild<MemoryWrapper*>(name);
  if(!mem) {
    mem = new QMemoryModel(this);
  }

  return mem;
}

MemoryWrapper *MemoryManager::Get(const QString &name)
{
  return Add(name);
}

void MemoryManager::Del(const QString &name)
{
  auto mem = this->findChild<MemoryWrapper*>(name);
  if(mem) {
    delete mem;
  }
}

MemoryWrapper *MemoryManager::globalMemory()
{
  return globalMemory_;
}
