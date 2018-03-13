#include "memorywrapper.h"
#include "tmemory.h"
#include "memorymanager.h"

//  MemoryWrapper

MemoryWrapper::MemoryWrapper(QObject *parent) : QObject(parent),//QMemoryModel(parent),
  mem_(new Memory::TMemory())
{

}

MemoryWrapper::~MemoryWrapper()
{
  clearDeleted();
  clearMeWrappers();
}

MEWrapper MemoryWrapper::add(const MEWrapper &parent, const QString &name, bool checkExist)
{
  MEWrapper me;
  auto p = parent;
  if(p.isNull())
    p = getME();
  if(p)
  {
    //me = add1(parent, name, checkExist);
    auto meParent = p.getMe();
    if(!meParent)
      return me;

    if(!name.isEmpty() && checkExist)
      me = p.get(name);

    if(me.isNull())
    {
      me = CreateMEW(meParent->Add(name));
      if(me) {
        doChange(me, EMemoryChange::mcAdd);
      }
    }
  }

  return me;
}

bool MemoryWrapper::addFrom(const MEWrapper &parent, const MEWrapper &mefrom, bool recurs, bool checkExist)
{
  bool  res = false;

  auto p = parent;
  if(p.isNull())
    p = getME();

  if(p && mefrom)
  {
    res = addFrom1(p.me_, mefrom.me_, recurs, checkExist);
  }

  return res;
}

bool MemoryWrapper::addFrom1(Memory::TME *parent, Memory::TME *mefrom, bool recurs, bool checkExist)
{
  if(!mefrom || ! parent)
    return false;

  bool res = parent->addFrom(mefrom, recurs, checkExist);

  if(res)
    doChange(CreateMEW(parent), EMemoryChange::mcAddFrom);

  return res;
}

void MemoryWrapper::del(const QString &path)
{
  auto me = get(path);

  if(me.isNull() || me.parent().isNull())
    return;

  //me->parent()->del(me->name());
  deleteMe(me);

}

void MemoryWrapper::deleteMe(const MEWrapper &me)
{
  if(!me.isNull())
  {
    deleteMe1(me);
  }
}

void MemoryWrapper::deleteMe1(const MEWrapper &me)
{
  if(!me.isNull())
  {
    ChangeEvent ev;
    ev.type = EMemoryChange::mcDel;
    ev.me = me;
    ev.me.me_ = nullptr;
    ev.parent = me.parent();
    ev.row = me.getIndex();
    //ev.count = me->count();

    auto me1 = me.getMe();
    ev.me.clearR(me1);
    ev.parent.getMe()->Del(me1);

    doChange(ev);
  }
}

MEWrapper MemoryWrapper::getME()
{
  return CreateMEW(mem_->getTopME());
}

void MemoryWrapper::addCount(const MEWrapper &parent, int count)
{
  auto p = parent;
  if(!p)
     p = getME();
  mem_->createNew(p.getMe(), count);

  doChange(parent, mcAddFrom);
}

MEWrapper MemoryWrapper::get(const QString &path)
{
  return CreateMEW(mem_->get(path));
}

MEWrapper MemoryWrapper::getById(uint id)
{
  auto me = reinterpret_cast<Memory::TME*>(id);
  MEWrapper resMe;
  if(!me)
    return resMe;
  if(!map_mew_.contains(me))
    return resMe;
  return map_mew_[me];
}

bool MemoryWrapper::getAutosave() const
{
  return mem_->getAutosave();
}

void MemoryWrapper::setAutosave(bool autosave)
{
  mem_->setAutosave(autosave);
}

bool MemoryWrapper::open(const QString &fileName)
{
  if(!mem_->open(fileName))
    return false;

  setSelected(getSelected());

  return true;
}

bool MemoryWrapper::save()
{
  if(mem_->getChanged())
    return mem_->save();
  return false;
}

QString MemoryWrapper::getFilePath() const
{
  return mem_->getFilePath();
}

void MemoryWrapper::setFilePath(const QString &file_path)
{
  mem_->setFilePath(file_path);
}

QVariant MemoryWrapper::getVal(const QString &path)
{
  auto me = get(path);
  if(me)
    return me.val();

  return QVariant();
}

void MemoryWrapper::setVal(const MEWrapper &me, const QVariant &val)
{
  if(me)
  {
    if(val == me.val())
      return;

    setVal1(me, val);
  }
}

void MemoryWrapper::setVal1(const MEWrapper &me, const QVariant &val)
{
  if(!me || !me.me_)
    return;

  ChangeEvent ev;
  ev.type = EMemoryChange::mcEditVal;
  ev.me = me;
  ev.prevVal = me.val();

  me.me_->setVal(val);

  doChange(ev);
}

void MemoryWrapper::setName(const MEWrapper &me, const QString &name)
{
  if(me)
  {
    if(name == me.name())
      return;

    setName1(me, name);
  }
}

void MemoryWrapper::setName1(const MEWrapper &me, const QString &name)
{
  if(!me || !me.me_)
    return;

  ChangeEvent ev;
  ev.type = EMemoryChange::mcEditName;
  ev.me = me;
  ev.prevName = me.name();

  me.me_->setName(name);

  doChange(ev);
}

void MemoryWrapper::setSelected(const MEWrapper &me)
{
  if(!me)
    return;

  if(mem_->getSelected() != me.me_) {
    mem_->setSelected(me.me_);

  }
  doChange(me, EMemoryChange::mcSelect);
}

MEWrapper MemoryWrapper::getSelected()
{
  return CreateMEW(mem_->getSelected());
}

void MemoryWrapper::doChange(const MEWrapper &me, EMemoryChange idMsg)
{
  bool changed = idMsg !=EMemoryChange::mcSelect && idMsg != EMemoryChange::mcUpdate;
  if(changed)
  {
    mem_->setChanged(changed);

    //if(mem_->autosave())
      //mem_->save();//����� ��������� ������ ���������
  }

  if(canChange_) {
    emit on_change(me, idMsg);

    ChangeEvent ev;
    ev.type = idMsg;
    ev.me = me;
    ev.parent = me.parent();
    ev.row = me.getIndex();
    ev.count = me.count();
    emit change(ev);
  }
}

void MemoryWrapper::doChange(const ChangeEvent &ev)
{
  bool changed = ev.type !=EMemoryChange::mcSelect && ev.type != EMemoryChange::mcUpdate;
  if(changed)
  {
    mem_->setChanged(changed);

    //if(mem_->autosave())
      //mem_->save();//����� ��������� ������ ���������
  }

  if(canChange_) {
    emit on_change(ev.me, ev.type);

    emit change(ev);
  }
}

void MemoryWrapper::clear()
{
  setSelected(getME());
  clearMe(getME());
}

void MemoryWrapper::clearMe(const MEWrapper &me)
{
  if(me) {
    clearMe1(me);
  }
}

void MemoryWrapper::clearR(Memory::TME *me)
{
  DeleteMEW(me);

  Memory::TME::Elements &childs = me->getElements();
  for(int i = 0, cnt = childs.count(); i <cnt; ++i)
  {
    auto me1 = childs.get(i);
    clearR(me1);
  }

  me->clear();
}

void MemoryWrapper::clearMe1(const MEWrapper &me)
{
  if(me && me.getMe()) {
    doChange(me, EMemoryChange::mcClear);

    Memory::TME::Elements &childs = me.getMe()->getElements();
    int cnt = childs.count();
    for(int i = 0; i <cnt; ++i)
    {
      clearR(childs.get(i));
    }
    me.getMe()->clear();
  }
}

bool MemoryWrapper::move(const MEWrapper &me, const MEWrapper &parent, int pos)
{
  if(me && parent) {
    return move1(me, parent, pos);
  }

  return false;
}

bool MemoryWrapper::move1(const MEWrapper &me, const MEWrapper &parent, int pos)
{
  if(!me || !parent)
    return false;
  // ��������� ������� �� ������ ��������� � �������
  if(me.parent() != parent)
    return false;

  bool ok = me.getMe()->move_to(parent.getMe(), pos);
  if(ok)
    doChange(me, mcMove);

  return ok;
}

bool MemoryWrapper::getCanChange() const
{
  return canChange_;
}

void MemoryWrapper::setCanChange(bool val)
{
  canChange_ = val;
}

bool MemoryWrapper::changed() const
{
  return mem_->getChanged();
}

void MemoryWrapper::undoBackup()
{
  if(mem_->undo())
  {
    mem_->setChanged(false);
    doChange(getME(), EMemoryChange::mcUpdate);
  }
}

void MemoryWrapper::redoBackup()
{
  if(mem_->redo())
  {
    mem_->setChanged(false);
    doChange(getME(), EMemoryChange::mcUpdate);
  }
}

bool MemoryWrapper::canUndoBackup()
{
  return mem_->canUndo();
}

bool MemoryWrapper::canRedoBackup()
{
  return mem_->canRedo();
}

void MemoryWrapper::clearDeleted()
{
  foreach (auto me, deleted_) {
    delete me;
  }

  deleted_.clear();
}

void MemoryWrapper::clearMeWrappers()
{
  map_mew_.clear();
}

MEWrapper MemoryWrapper::CreateMEW(Memory::TME *me)
{
  MEWrapper resMe;
  if(!me)
    return resMe;

  if(!map_mew_.contains(me))
    map_mew_[me] = MEWrapper(me, this);

  return map_mew_[me];
}

void MemoryWrapper::DeleteMEW(Memory::TME *me)
{
  if(!me)
    return;

  if(map_mew_.contains(me))
  {
    map_mew_.remove(me);
  }
}
