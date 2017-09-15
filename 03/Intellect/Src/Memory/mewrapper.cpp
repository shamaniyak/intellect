#include "mewrapper.h"
#include "tme.h"
#include "memorywrapper.h"

MEWrapper::MEWrapper(MemoryWrapper *mem)// : QObject(parent)
{
  setMem(mem);
}

MEWrapper::MEWrapper(Memory::TME *me, MemoryWrapper *mem) : //QObject(parent),
  me_(me)
{
  setMem(mem);
}

MEWrapper::~MEWrapper()
{

}

void MEWrapper::clearR(Memory::TME *me)
{
  mem_->DeleteMEW(me);

  Memory::TME::Elements &childs = me->getElements();
  for(int i = 0, cnt = childs.count(); i <cnt; ++i)
  {
    auto me1 = childs.get(i);
    clearR(me1);
  }

  me->clear();
}

void MEWrapper::clear()
{
  if(mem_ && me_) {
    mem_->doChange(this, EMemoryChange::mcClear);

    Memory::TME::Elements &childs = me_->getElements();
    int cnt = childs.count();
    for(int i = 0; i <cnt; ++i)
    {
      clearR(childs.get(i));
      //mem_->DeleteMEW(childs.get(i));
    }
    me_->clear();
  }
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

void MEWrapper::deleteMe(MEWrapper *me)
{
  if(me && mem_ && me_)
  {
    ChangeEvent ev;
    ev.type = EMemoryChange::mcDel;
    ev.me = me;
    ev.parent = me->parent();
    ev.row = me->getIndex();
    ev.count = me->count();

    auto me1 = me->me_;
    clearR(me1);
    me_->Del(me1);

    mem_->doChange(ev);
    //mem_->DeleteMEW(me->me_);
    // из-за того что мы не можем обращатьс€ к самому элементу, т. к. он удален,
    // то мы просто обновл€ем его владельца
    //mem_->doChange(this, EMemoryChange::mcUpdate);
    //mem_->mem_->setChanged(true);

    //mem_->setSelected(this);
  }
}

QString MEWrapper::name() const
{
  if(!me_)
    return "";
  return me_->name();
}

void MEWrapper::setName(const QString &name)
{
  if(!me_)
    return;
  if(name == this->name())
    return;

  me_->setName(name);

  if(mem_) {
    mem_->doChange(this, EMemoryChange::mcEditName);
  }
}

QVariant MEWrapper::val() const
{
  if(!me_)
    return QVariant();
  return me_->val();
}

void MEWrapper::setVal(const QVariant &val)
{
  if(!me_)
    return;
  if(val == this->val())
    return;

  me_->setVal(val);

  if(mem_) {
    mem_->doChange(this, EMemoryChange::mcEditVal);
  }
}

QString MEWrapper::getPath() const
{
  if(!me_)
    return "";
  return me_->getPath();
}

MEWrapper *MEWrapper::add(const QString &name, bool checkExist)
{
  if(mem_)
    return mem_->add(this, name);
  return nullptr;

//  if(!me_)
//    return nullptr;
//  MEWrapper *me = nullptr;

//  if(!name.isEmpty() && checkExist)
//    me = get(name);

//  if(!me) {
//    if(mem_) {
//      me = mem_->CreateMEW(me_->Add(name));
//      if(me)
//        mem_->doChange(me, EMemoryChange::mcAdd);
//    }
//  }

//  return me;
}

MEWrapper *MEWrapper::get(const QString &name)
{
  if(mem_ && me_)
    return mem_->CreateMEW(me_->Get(name));
  return nullptr;
}

MEWrapper *MEWrapper::getByI(int i)
{
  if(mem_ && me_) {
    return mem_->CreateMEW(me_->getElements().get(i));
  }

  return nullptr;
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

void MEWrapper::delByMe(MEWrapper *me)
{
  deleteMe(me);
}

MEWrapper *MEWrapper::parent() const
{
  if(mem_ && me_)
    return mem_->CreateMEW(me_->parent());
  return nullptr;
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

bool MEWrapper::deleted() const
{
  return(!me_);
}
