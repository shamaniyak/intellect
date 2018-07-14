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

#include "iobject.h"
#include "talgorithm.h"

TAlgorithm::TAlgorithm(QObject *parent) : QObject(parent),
  MM_(new MemoryManager(this)),
  OM_(new ObjectManager(this)),
  PM_(new PluginManager(this)),
  WM_(new WindowManager(this)),
  qml_(new QmlManager(this))
{
}

TAlgorithm::~TAlgorithm()
{

}

bool TAlgorithm::addObject(QObject *obj, const QString &name)
{
  IObject *iobj = getObject(name);

  if(iobj) {
    return( iobj->addObject(obj, name) );
  }

  return false;
}

IObject *TAlgorithm::getObject(const QString &name)
{
  return qobject_cast<IObject*>(OM_->Get(name));
}

QmlManager *TAlgorithm::getQml() const
{
  return qml_;
}

MemoryManager *TAlgorithm::getMM() const
{
    return MM_;
}

WindowManager *TAlgorithm::getWM() const
{
  return WM_;
}

PluginManager *TAlgorithm::getPM() const
{
  return PM_;
}

ObjectManager *TAlgorithm::getOM() const
{
    return OM_;
}
