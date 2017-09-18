#include "memorywrapper.h"
#include "tmemory.h"

#include <QUndoStack>

class BaseCommand : public QUndoCommand
{
public:
  explicit BaseCommand(MemoryWrapper *m, QUndoCommand *parent = Q_NULLPTR) :
    QUndoCommand(parent), m_(m) {}
  explicit BaseCommand(MemoryWrapper *m, const QString &text, QUndoCommand *parent = Q_NULLPTR) :
    QUndoCommand(text, parent), m_(m) {}
  virtual ~BaseCommand() {}

protected:
  MemoryWrapper *m_ = nullptr;
};

class AddCommand : public BaseCommand
{
  // QUndoCommand interface
public:
  AddCommand(MemoryWrapper *m, MEWrapper *parent, const QString &name) :
    BaseCommand(m, "Add"), parent_(parent), name_(name) {}
  virtual void undo() override
  {
    m_->doChange(m_->CreateMEW(newMe_), mcDel);
    parent_->getMe()->Del(newMe_);
  }
  virtual void redo() override
  {
    newMe_ = parent_->getMe()->Add(name_);
    m_->doChange(m_->CreateMEW(newMe_), mcAdd);
  }

  Memory::TME *newMe() const { return newMe_; }

private:
  MEWrapper *parent_ = nullptr;
  QString name_;
  Memory::TME *newMe_ = nullptr;
};

class AddFromCommand : public BaseCommand
{
  // QUndoCommand interface
public:
  virtual void undo() override
  {
  }
  virtual void redo() override
  {
  }
};

class DelCommand : public BaseCommand
{
  // QUndoCommand interface
public:
  virtual void undo() override
  {
  }
  virtual void redo() override
  {
  }
};

class EditNameCommand : public BaseCommand
{
  // QUndoCommand interface
public:
  virtual void undo() override
  {
  }
  virtual void redo() override
  {
  }
};

class EditValCommand : public BaseCommand
{
  // QUndoCommand interface
public:
  virtual void undo() override
  {
  }
  virtual void redo() override
  {
  }
};

class ClearCommand : public BaseCommand
{
  // QUndoCommand interface
public:
  virtual void undo() override
  {
  }
  virtual void redo() override
  {
  }
};

class MoveCommand : public BaseCommand
{
  // QUndoCommand interface
public:
  virtual void undo() override
  {
  }
  virtual void redo() override
  {
  }
};

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

MEWrapper *MemoryWrapper::add(MEWrapper *parent, const QString &name, bool checkExist)
{
  if(!parent)
    parent = getME();

  MEWrapper *me = nullptr;

  auto meParent = parent->getMe();
  if(!meParent)
    return me;

  if(!name.isEmpty() && checkExist)
    me = parent->get(name);

  if(!me)
  {
    auto s = getStack();
    if(s) {
      AddCommand *cmd = new AddCommand(this, parent, name);
      s->push(cmd);
      me = CreateMEW(cmd->newMe());
    }
    else
    {
      me = CreateMEW(meParent->Add(name));
      if(me)
        doChange(me, EMemoryChange::mcAdd);
    }
  }

  return me;
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

MEWrapper *MemoryWrapper::getSelected()
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
  getSelected()->clear();
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

bool MemoryWrapper::getCanChange() const
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

bool MemoryWrapper::getCanUndo() const
{
  return canUndo_;
}

void MemoryWrapper::setCanUndo(bool canUndo)
{
  canUndo_ = canUndo;
  if(!canUndo_)
    delete stack_;
}

QUndoStack *MemoryWrapper::getStack()
{
  if(!stack_ && canUndo_)
    stack_ = new QUndoStack(this);
  return stack_;
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





