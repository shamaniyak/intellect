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

#include "objectmanager.h"
#include "iobject.h"

ObjectManager::ObjectManager(QObject *parent) : QObject(parent)
{

}

void ObjectManager::abort()
{
  t_objects::iterator it = objects_.begin();
  for(; it != objects_.end(); ++it)
  {
    it.value()->scr()->abort();
  }
}

void ObjectManager::CreateObject(const QString &name)
{
  IObject *iobj = new IObject(this);
  objects_[name] = iobj;

  iobj->setObjectName(name);
}

QObject *ObjectManager::Add(const QString &name)
{
  if(name.isEmpty())
    return nullptr;

  QString upperName = name.toUpper();

  bool contains = objects_.contains(upperName);
  if(!contains) {
    CreateObject(upperName);
  }

  auto obj = qobject_cast<IObject*>( objects_[upperName] );
  obj->incCountLinks();

  if(!contains)
    emit signalAddObject(obj);

  return obj;
}

QObject *ObjectManager::Get(const QString &name)
{
  return Add(name);
}

bool ObjectManager::Del(const QString &name)
{
  QObject *temp = nullptr;
  QString upperName = name.toUpper();

  if(objects_.contains(upperName)) {
    temp = objects_[upperName];
    delete temp;
    objects_.remove(upperName);
  }

  return temp;
}
