#include "memorywrapper.h"
#include "tmemory.h"

MemoryWrapper::MemoryWrapper(QObject *parent) : QObject(parent),//QMemoryModel(parent),
  mem_(new Memory::TMemory())
{

}

MemoryWrapper::~MemoryWrapper()
{
  clearDeleted();
  clearMeWrappers();
}

MEWrapper *MemoryWrapper::add(MEWrapper *parent, const QString &name)
{
  if(!parent)
    parent = getME();

  MEWrapper *newME = parent->add(name);

  return newME;
}

bool MemoryWrapper::addFrom(MEWrapper *parent, MEWrapper *mefrom, bool recurs)
{
  if(!mefrom)
    return false;

  if(!parent)
    parent = getME();

  bool res = mem_->addFrom(parent->me_, mefrom->me_, recurs);

  if(res)
    doChange(parent, EMemoryChange::mcAddFrom);

  return res;
}

void MemoryWrapper::del(const QString &path)
{
  auto me = get(path);

  if(!me || !me->parent())
    return;

  me->parent()->del(me->name());

}

MEWrapper *MemoryWrapper::getME()
{
  return CreateMEW(mem_->getTopME());
}

void MemoryWrapper::addCount(MEWrapper *parent, int count)
{
  if(!parent)
     parent = getME();
  mem_->createNew(parent->getMe(), count);

  doChange(parent, mcAddFrom);
}

MEWrapper *MemoryWrapper::get(const QString &path)
{
  return CreateMEW(mem_->get(path));
}

MEWrapper *MemoryWrapper::getById(uint id)
{
  auto me = reinterpret_cast<Memory::TME*>(id);
  if(!me)
    return nullptr;
  if(!map_mew_.contains(me))
    return nullptr;
  return map_mew_[me];
}

bool MemoryWrapper::autosave() const
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

  setSelected(selected());

  return true;
}

bool MemoryWrapper::save()
{
  if(mem_->getChanged())
    return mem_->save();
  return false;
}

QString MemoryWrapper::file_path() const
{
  return mem_->getFilePath();
}

void MemoryWrapper::setFile_path(const QString &file_path)
{
  mem_->setfilePath(file_path);
}

QVariant MemoryWrapper::getVal(const QString &path)
{
  auto me = get(path);
  if(me)
    return me->val();

  return QVariant();
}

void MemoryWrapper::setSelected(MEWrapper *me)
{
  if(!me)
    return;

  mem_->setSelected(me->me_);

  doChange(me, EMemoryChange::mcSelect);
}

MEWrapper *MemoryWrapper::selected()
{
  return CreateMEW(mem_->getSelected());
}

void MemoryWrapper::doChange(MEWrapper *me, EMemoryChange idMsg)
{
  bool changed = idMsg !=EMemoryChange::mcSelect && idMsg != EMemoryChange::mcUpdate;
  if(changed)
  {
    mem_->setChanged(changed);

    //if(mem_->autosave())
      //mem_->save();//будем сохранять каждое изменение
  }

  if(mem_->canChange()) {
    emit on_change(me, idMsg);

    ChangeEvent ev;
    ev.type = idMsg;
    ev.me = me;
    ev.parent = me->parent();
    ev.row = me->getIndex();
    ev.count = me->count();
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
      //mem_->save();//будем сохранять каждое изменение
  }

  if(mem_->canChange()) {
    emit on_change(ev.me, ev.type);

    emit change(ev);
  }
}

void MemoryWrapper::clear()
{
  setSelected(getME());
  selected()->clear();
  //mem_->clear();

  //doChange(getME(), EMemoryChange::mcClear);
}

void MemoryWrapper::move(MEWrapper *me, MEWrapper *parent, int pos)
{
  if(!me || !parent)
    return;

  bool ok = me->getMe()->move_to(parent->getMe(), pos);
  if(ok)
    doChange(me, mcMove);
}

bool MemoryWrapper::canChange() const
{
  return mem_->canChange();
}

void MemoryWrapper::setCanChange(bool val)
{
  mem_->setCanChange(val);
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
  foreach (auto me, map_mew_) {
    delete me;
  }

  map_mew_.clear();
}

MEWrapper *MemoryWrapper::CreateMEW(Memory::TME *me)
{
  if(!me)
    return nullptr;

  if(!map_mew_.contains(me))
    map_mew_[me] = new MEWrapper(me, this);

  return map_mew_[me];
}

void MemoryWrapper::DeleteMEW(Memory::TME *me)
{
  if(!me)
    return;

  if(map_mew_.contains(me))
  {
    auto mew = map_mew_[me];
    //delete mew;
    map_mew_.remove(me);
    mew->me_ = nullptr;
    deleted_.push_back(mew);
  }
}



std::shared_ptr<MemoryLogger> MemoryWrapper::getLogger()
{
  if(!logger_.get())
    logger_ = std::make_shared<MemoryLogger>(this);
  return logger_;
}
