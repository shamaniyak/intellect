#include "mewrapper.h"
#include "tme.h"
#include "memorywrapper.h"

MEWrapper::MEWrapper()
{

}

MEWrapper::MEWrapper(MemoryWrapper *mem)// : QObject(parent)
{
  setMem(mem);
}

MEWrapper::MEWrapper(Memory::TME *me, MemoryWrapper *mem) : //QObject(parent),
  me_(me)
{
  setMem(mem);
}

MEWrapper::MEWrapper(const MEWrapper &src)
{
  this->mem_ = src.mem_;
  this->me_ = src.me_;
}

MEWrapper::~MEWrapper()
{

}

void MEWrapper::clear()
{
  if(mem_)
    mem_->clearMe(*this);
}

Memory::TME *MEWrapper::getMe() const
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

void MEWrapper::deleteMe(MEWrapper &me)
{
  if(mem_)
    mem_->deleteMe(me);
}

QString MEWrapper::name() const
{
  if(!me_)
    return "";
  return me_->name();
}

void MEWrapper::setName(const QString &name)
{
  if(mem_)
    mem_->setName(*this, name);
}

QVariant MEWrapper::val() const
{
  if(!me_)
    return QVariant();
  return me_->val();
}

void MEWrapper::setVal(const QVariant &val)
{
  if(mem_)
    mem_->setVal(*this, val);
}

QString MEWrapper::getPath() const
{
  if(!me_)
    return "";
  return me_->getPath();
}

MEWrapper MEWrapper::add(const QString &name, bool checkExist)
{
  if(mem_)
    return mem_->add(*this, name, checkExist);
  return MEWrapper();
}

bool MEWrapper::addFrom(MEWrapper &from, bool recurs)
{
  if(mem_)
    return mem_->addFrom(*this, from, recurs);
  return false;
}

MEWrapper MEWrapper::get(const QString &name)
{
  if(mem_ && me_)
    return mem_->CreateMEW(me_->Get(name));
  return MEWrapper();
}

MEWrapper MEWrapper::getByI(int i)
{
  if(mem_ && me_) {
    return mem_->CreateMEW(me_->getElements().get(i));
  }

  return MEWrapper();
}

void MEWrapper::del(const QString &name)
{
  if(!me_)
    return;
  auto me = get(name); //me_->Get(name);
  deleteMe(me);
}

void MEWrapper::delByI(int i)
{
  if(!me_)
    return;
  auto me = getByI(i);
  deleteMe(me);
}

void MEWrapper::delByMe(MEWrapper &me)
{
  deleteMe(me);
}

MEWrapper MEWrapper::parent() const
{
  if(mem_ && me_)
    return mem_->CreateMEW(me_->parent());
  return MEWrapper();
}

int MEWrapper::count() const
{
  if(!me_)
    return 0;
  return me_->getElements().count();
}

int MEWrapper::getIndex() const
{
  if(!me_)
    return -1;
  return me_->get_index();
}

bool MEWrapper::isNull() const
{
  return(!me_);
}
