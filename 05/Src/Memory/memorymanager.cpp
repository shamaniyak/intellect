/****************************************************************************
**
** Copyright (C) 2015-2018 Aleksandr Abramov
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
** http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
****************************************************************************/

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
