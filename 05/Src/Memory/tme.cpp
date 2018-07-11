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

#include "tme.h"
#include "tmemory.h"

//#include <algorithm>
#include <QDebug>

namespace Memory
{

static int countInstances = 0;

TME::TME():
  id_name_(-1)
{
  ++countInstances;
  //qDebug() << "TME" << countInstances;
}

TME::TME(shared_me parent, int id_name, QVariant val):
  id_name_(id_name), val_(val), parent_(parent)
{
  ++countInstances;
  //qDebug() << "TME" << countInstances;
}

TME::TME(const TME &me)
{
  ++countInstances;
  //qDebug() << "TME" << countInstances;
  parent_ = me.parent_;
  *this = me;
}

TME::~TME()
{
  --countInstances;
  //qDebug() << "~TME()" << this << countInstances;
  clear();
}

TME &TME::operator=(const TME &me)
{
  val_ = me.val_;
  id_name_ = me.id_name_;
  return *this;
}

QString TME::name() const
{
  TMemory *m = this->mem();
  if(m)
    return m->getWord(id_name_);
  return QString();
}

void TME::setName(const QString &name)
{
  TMemory *m = this->mem();
  if(m) {
    id_name_ = m->getWordIdx(name);
  }
}

QVariant TME::val() const
{
  return val_.val();
}

void TME::setVal(const TMEValue &val)
{
  val_ = val;
}

void TME::setVal(const QVariant &val)
{
  val_.setVal(val);
}

TME::shared_me TME::parent() const
{
  return parent_.lock();
}

QString TME::path() const
{
  QString res;
  auto p = parent_.lock();
  if(p)
    res = p->path() + "\\";
  res += name();
  return res;
}

void TME::add(const TME::shared_me &me)
{
    childs_.add(me);
}

bool TME::addFrom(shared_me mefrom, shared_me me_to, bool recurs, bool checkExist)
{
  if(!mefrom || !me_to)
    return false;
  bool res = false;

  auto elements = mefrom->getElements();
  for(int i =0; i <elements.count(); ++i)
  {
    auto me1 = elements.get(i);
    shared_me me2;
    if(checkExist)
      me2 = me_to->get(me1->name());
    if(!me2) {
      auto m = mem();
      int id = m ? m->getWordIdx(me1->name()) : me1->id_name();
      me2 = TME::create(me_to, id, me1->val());
    }
    if(recurs)
    {
      res = addFrom(me1, me2, recurs, checkExist);
    }
  }

  return true;
}

TME::shared_me TME::get(const QString &name)
{
  int idx =-1;
  TMemory *m = this->mem();

  if(m)
    idx = m->getWordIdx(name);

  if(idx >=0)
    return childs_.get_by_id_name(idx);

  return {};
}

bool TME::del(const QString &name)
{
  auto me = get(name);
  return del(me);
}

bool TME::del(const shared_me &me)
{
  if(me) {
    return( childs_.remove(me) );
  }
  return false;
}

void TME::clear()
{
  childs_.clear();
}

int TME::getIndex() const
{
  int res = -1;
  auto p = parent_.lock();
  if(p)
  {
    res = p->childs_.get_index(this);
  }
  return res;
}

bool TME::move_to(int pos)
{
  if(pos <0)
    return false;

  shared_me p = parent_.lock();
  if(!p)
    return false;
  return p->childs_.move(getIndex(), pos);
}

void TME::load(QDataStream &ds)
{
  ds >> id_name_;
  val_.load(ds);
  //childs_.load(ds, this);
}

void TME::save(QDataStream &ds) const
{
  ds << id_name_;
  val_.save(ds);
  childs_.save(ds);
}

int TME::id_name() const
{
  return id_name_;
}

void TME::setIdName(int id)
{
  id_name_ = id;
}

TME::Elements &TME::getElements()
{
  return childs_;
}

TMemory *TME::mem() const
{
  if(!parent_.lock()) {
    auto topMe = reinterpret_cast<const TopME*>(this);
    return (topMe ? topMe->mem() : nullptr);
  }
  else
    return parent_.lock()->mem();
}

TMEData TME::data() const
{
  TMEData data;
  data.name_ = name();
  data.val_ = val();
  return data;
}

int TME::size()
{
  int res = 0;//sizeof(id_name_);
  //res += sizeof(val_);
  res += sizeof(std::weak_ptr<TME>);
  //res += sizeof(elements_svec);
  //res += sizeof(std::shared_ptr<Memory::TME>);
  return res;
}

TME::shared_me TME::create(shared_me parent, int id, QVariant val)
{
  shared_me me = std::make_shared<TME>(parent, id, val);
  if(parent) parent->add(me);
  return me;
}

void TME::setParent(shared_me parent)
{
  parent_ = parent;
}

TME::Elements::Elements()
{
  //items_.reserve(10000);
}

void TME::Elements::add(const shared_me &me)
{
  items_.push_back(me);
}

int TME::Elements::count() const
{
  return items_.size();
}

TME::shared_me TME::Elements::get(int i) const
{
  if(i <0 || i >=count())
    return {};
  return items_[i];
}

TME::shared_me TME::Elements::get_by_id_name(int id) const
{
  for(const auto &mei: items_)
  {
    if(mei->id_name_ ==id)
    {
      return mei;
    }
  }
  return {};
}

bool TME::Elements::remove(const shared_me me)
{
  auto it = std::find(items_.begin(), items_.end(), me);
  if(it != items_.end())
  {
    items_.erase(it);
    return true;
  }
  return false;
}

void TME::Elements::clear()
{
  items_.clear();
}

int TME::Elements::get_index(const shared_me me) const
{
  auto it = std::find(items_.begin(), items_.end(), me);
  if(it !=items_.end())
    return std::distance(items_.begin(), it);
  return -1;
}

int TME::Elements::get_index(const TME *me) const
{
  auto it = std::find_if(items_.begin(), items_.end(), [&me](shared_me x){return(x.get()==me);});
  if(it !=items_.end())
    return std::distance(items_.begin(), it);
  return -1;
}

bool TME::Elements::move(int from, int to)
{
  if(from !=to && from >=0 && from <count() && to >=0 && to <count())
  {
    int delta = 1;
    if(from > to)
      delta = -1;
    auto x = items_.at(from);
    while(from != to)
    {
      items_[from] = items_[from + delta];
      from += delta;
    }
    items_[to] = x;

    //items_.insert(items_.begin() +to, x);
    //items_.erase(items_.begin() +from);
    return true;
  }
  return false;
}

void TME::Elements::load(QDataStream &ds, shared_me parent)
{
  int cnt = 0;
  ds >> cnt;

  for(int i =0; i <cnt; ++i)
  {
    auto me = TME::create(parent);
    me->load(ds);
    me->getElements().load(ds, me);
  }
}

void TME::Elements::save(QDataStream &ds) const
{
 int cnt = count();
 ds << cnt;

 for(auto me: items_)
 {
   me->save(ds);
 }
}

}
