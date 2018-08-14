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
