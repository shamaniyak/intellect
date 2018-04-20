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
  if(!isNull())
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
  if(!isNull())
    mem_->deleteMe(me);
}

QString MEWrapper::name() const
{
  if(isNull())
    return "";
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
    return "";
  return me_->getPath();
}

MEWrapper MEWrapper::add(const QString &name, bool checkExist)
{
  if(!isNull())
    return mem_->add(*this, name, checkExist);
  return MEWrapper();
}

bool MEWrapper::addFrom(MEWrapper &from, bool recurs)
{
  if(!isNull())
    return mem_->addFrom(*this, from, recurs);
  return false;
}

MEWrapper MEWrapper::get(const QString &name)
{
  if(!isNull())
    return mem_->CreateMEW(me_->Get(name));
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
  auto me = get(name); //me_->Get(name);
  deleteMe(me);
}

void MEWrapper::delByI(int i)
{
  if(isNull())
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
  return me_->get_index();
}

bool MEWrapper::isNull() const
{
  if(me_ && mem_)
    return false;
    //return(!mem_->getById(reinterpret_cast<uint>(me_)));
  return(true);
}
