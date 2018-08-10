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

#include "mewrapper.h"
#include "tme.h"
#include "memorywrapper.h"

#include <QDebug>

MEWrapper::MEWrapper()
{

}

MEWrapper::MEWrapper(MemoryWrapper *mem)
{
  setMem(mem);
}

MEWrapper::MEWrapper(const Memory::TME::shared_me &me, MemoryWrapper *mem) :
  me_(me)
{
  //if(me_) qDebug() << "MEWrapper" << me_.get() << me_.use_count();
  setMem(mem);
}

MEWrapper::MEWrapper(const MEWrapper &src)
{
  *this = src;
}

MEWrapper::~MEWrapper()
{
  //if(me_) qDebug() << "~MEWrapper" << me_.get() << me_.use_count()-1;
}

Memory::TME::shared_me MEWrapper::getMe() const
{
  return me_;
}

MemoryWrapper *MEWrapper::getMem() const
{
  return mem_;
}

void MEWrapper::setMem(MemoryWrapper *mem)
{
  mem_ = mem;
}

void MEWrapper::deleteMe(const MEWrapper &me)
{
  if(!isNull())
    mem_->deleteMe(me);
}

QString MEWrapper::name() const
{
  if(isNull())
    return QString();
  return me_->name();
}

void MEWrapper::setName(const QString &name)
{
  if(!isNull())
    mem_->setName(*this, name);
}

QVariant MEWrapper::val() const
{
  if(isNull())
    return QVariant();
  return me_->val();
}

void MEWrapper::setVal(const QVariant &val)
{
  if(!isNull())
    mem_->setVal(*this, val);
}

QString MEWrapper::getPath() const
{
  if(isNull())
    return QString();
  return me_->path();
}

MEWrapper MEWrapper::add(const QString &name, bool checkExist)
{
  if(!isNull())
    return mem_->add(*this, name, checkExist);
  return MEWrapper();
}

bool MEWrapper::addFrom(const MEWrapper &from, bool recurs)
{
  if(!isNull())
    return mem_->addFrom(*this, from, recurs);
  return false;
}

MEWrapper MEWrapper::get(const QString &name)
{
  if(!isNull())
    return mem_->CreateMEW(me_->get(name));
  return MEWrapper();
}

MEWrapper MEWrapper::getByI(int i)
{
  if(!isNull()) {
    return mem_->CreateMEW(me_->getElements().get(i));
  }

  return MEWrapper();
}

void MEWrapper::del(const QString &name)
{
  if(isNull())
    return;
  auto me = get(name);
  deleteMe(me);
}

void MEWrapper::delByI(int i)
{
  if(isNull())
    return;
  auto me = getByI(i);
  deleteMe(me);
}

void MEWrapper::delByMe(const MEWrapper &me)
{
  deleteMe(me);
}

void MEWrapper::clear()
{
  if(!isNull())
      mem_->clearMe(*this);
}

void MEWrapper::move(const MEWrapper &parent, int pos)
{
    if(isValid())
        mem_->move(*this, parent, pos);
}

MEWrapper MEWrapper::parent() const
{
  if(!isNull())
    return mem_->CreateMEW(me_->parent());
  return MEWrapper();
}

int MEWrapper::count() const
{
  if(isNull())
    return 0;
  return me_->getElements().count();
}

int MEWrapper::getIndex() const
{
  if(!me_)
    return -1;
  return me_->parent() ? me_->parent()->getElements().get_index(me_) : -1;
}

bool MEWrapper::isNull() const
{
  if(me_ && mem_)
    return false;
  return(true);
}

bool MEWrapper::isValid() const
{
    return !isNull();
}

uint MEWrapper::getUid() const
{
  return reinterpret_cast<uint>(me_.get());
}

MEWrapper &MEWrapper::operator =(const MEWrapper &src)
{
    this->mem_ = src.mem_;
    this->me_ = src.me_;
    //qDebug() << "MEWrapper =" << me_.get() << me_.use_count();
    return *this;
}
