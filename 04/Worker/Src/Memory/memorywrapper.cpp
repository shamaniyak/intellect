#include "memorywrapper.h"
#include "tmemory.h"
#include "memorymanager.h"

#include <QUndoStack>

class BaseCommand : public QUndoCommand
{
public:
  explicit BaseCommand(MemoryWrapper *m, QUndoCommand *parent = Q_NULLPTR) : QUndoCommand(parent),
    m_(m) {  }
  explicit BaseCommand(MemoryWrapper *m, const QString &text, QUndoCommand *parent = Q_NULLPTR) : QUndoCommand(text, parent),
    m_(m) {  }
  virtual ~BaseCommand() {  }

protected:
  MemoryWrapper *m_ = nullptr;
};

class AddCommand : public BaseCommand
{
  // QUndoCommand interface
public:
  AddCommand(MemoryWrapper *m, MEWrapper *parent, const QString &name, bool checkExist) :
    BaseCommand(m, "Add"),
    parent_(parent), name_(name), checkExist_(checkExist) {   }

  virtual void undo() override
  {
    m_->deleteMe1(newMe_);
  }
  virtual void redo() override
  {
    newMe_ = m_->add1(parent_, name_, checkExist_);
  }

  MEWrapper *newMe() const { return newMe_; }

private:
  MEWrapper *parent_ = nullptr;
  QString name_;
  bool checkExist_ = true;
  MEWrapper *newMe_ = nullptr;
};

class AddFromCommand : public BaseCommand
{
public:
  AddFromCommand(MemoryWrapper *m, MEWrapper *parent, MEWrapper *from, bool recurs) : BaseCommand(m, "AddFrom"),
    parent_(parent), from_(from), recurs_(recurs)
  {
    auto items = from_->getMe()->getElements();
    int cnt = items.count();
    for(int i = 0; i < cnt; ++i)
      list_ << items.get(i)->name();
  }

  virtual void undo() override
  {
    foreach (auto name, list_) {
      parent_->del(name);
    }
  }
  virtual void redo() override
  {
    m_->addFrom1(parent_->getMe(), from_->getMe(), recurs_);
  }

private:
  MEWrapper *parent_ = nullptr;
  MEWrapper *from_ = nullptr;
  bool recurs_ = true;
  QStringList list_;
};

class DelCommand : public BaseCommand
{
public:
  DelCommand(MemoryWrapper *m, MEWrapper *me) : BaseCommand(m, "Del"),
    me_(me), parent_(me->parent()), name_(me->name()), index_(me->getIndex())
  {
    buf_.setMem(m->mem_.get());
    buf_.addFrom(me_->getMe(), true);
  }

  virtual void undo() override
  {
    me_ = m_->add1(parent_, name_, false);
    m_->move(me_, parent_, index_);
    m_->addFrom1(me_->getMe(), &buf_, true);
  }
  virtual void redo() override
  {
    m_->deleteMe1(me_);
  }

private:
  MEWrapper *me_ = nullptr;
  MEWrapper *parent_ = nullptr;
  QString name_;
  int index_;
  Memory::TopME buf_;
};

class EditNameCommand : public BaseCommand
{
public:
  EditNameCommand(MemoryWrapper *m, MEWrapper *me, const QString &name) :
    BaseCommand(m, "EditName"), me_(me), newName_(name), oldName_(me->name())
  {  }

  virtual void undo() override
  {
    me_->setName(oldName_);
  }
  virtual void redo() override
  {
    me_->setName(newName_);
  }

private:
  MEWrapper *me_ = nullptr;
  QString newName_, oldName_;
};

class EditValCommand : public BaseCommand
{
public:
  EditValCommand(MemoryWrapper *m, MEWrapper *me, const QVariant &val) :
    BaseCommand(m, "EditVal"), me_(me), newVal_(val), oldVal_(me->val())
  {  }

  virtual void undo() override
  {
    me_->setVal(oldVal_);
  }
  virtual void redo() override
  {
    me_->setVal(newVal_);
  }

private:
  MEWrapper *me_ = nullptr;
  QVariant newVal_, oldVal_;
};

class ClearCommand : public BaseCommand
{
public:
  ClearCommand(MemoryWrapper *m, MEWrapper *me) : BaseCommand(m, "Clear"),
    me_(me)
  {
    buf_.setMem(m->mem_.get());
    buf_.addFrom(me_->getMe(), true);
  }

  virtual void undo() override
  {
    m_->addFrom1(me_->getMe(), &buf_, true);
  }
  virtual void redo() override
  {
    m_->clearMe1(me_);
  }

private:
  MEWrapper *me_ = nullptr;
  Memory::TopME buf_;
};

class MoveCommand : public BaseCommand
{
public:
  MoveCommand(MemoryWrapper *m, MEWrapper *me, int index) : BaseCommand(m, "Move"),
    me_(me), newIndex_(index), oldIndex_(me->getIndex())
  {}

  virtual void undo() override
  {
    m_->move1(me_, me_->parent(), oldIndex_);
  }
  virtual void redo() override
  {
    m_->move1(me_, me_->parent(), newIndex_);
  }

private:
  MEWrapper *me_ = nullptr;
  int newIndex_, oldIndex_;
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
  MEWrapper *me = nullptr;
  auto s = getStack();
  if(s) {
    AddCommand *cmd = new AddCommand(this, parent, name, checkExist);
    s->push(cmd);
    me = cmd->newMe();
  }
  else
  {
    me = add1(parent, name, checkExist);
  }

  return me;
}

bool MemoryWrapper::addFrom(MEWrapper *parent, MEWrapper *mefrom, bool recurs)
{
  bool  res = false;

  if(!parent)
    parent = getME();

  if(parent && mefrom)
    res = addFrom1(parent->me_, mefrom->me_, recurs);

  return res;
}

bool MemoryWrapper::addFrom1(Memory::TME *parent, Memory::TME *mefrom, bool recurs)
{
  if(!mefrom || ! parent)
    return false;

  bool res = parent->addFrom(mefrom, recurs);

  if(res)
    doChange(CreateMEW(parent), EMemoryChange::mcAddFrom);
    //doChange(getById((uint)parent), EMemoryChange::mcAddFrom);

  return res;
}

void MemoryWrapper::del(const QString &path)
{
  auto me = get(path);

  if(!me || !me->parent())
    return;

  //me->parent()->del(me->name());
  deleteMe(me);

}

void MemoryWrapper::deleteMe(MEWrapper *me)
{
  if(me)
  {
    auto s = getStack();
    if(s) {
      DelCommand *cmd = new DelCommand(this, me);
      s->push(cmd);
      return;
    }

    deleteMe1(me);
  }
}

void MemoryWrapper::deleteMe1(MEWrapper *me)
{
  if(me)
  {
    auto parent = me->parent();

    ChangeEvent ev;
    ev.type = EMemoryChange::mcDel;
    ev.me = me;
    ev.parent = me->parent();
    ev.row = me->getIndex();
    //ev.count = me->count();

    auto me1 = me->getMe();
    me->clearR(me1);
    parent->getMe()->Del(me1);

    doChange(ev);
  }
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

  if(mem_->getSelected() != me->me_) {
    mem_->setSelected(me->me_);

  }
  doChange(me, EMemoryChange::mcSelect);
}

MEWrapper *MemoryWrapper::getSelected()
{
  return CreateMEW(mem_->getSelected());
}

void MemoryWrapper::createUndoStack()
{
  if(!stack_)
    stack_ = new QUndoStack(this);
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

  if(canChange_) {
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

  if(canChange_) {
    emit on_change(ev.me, ev.type);

    emit change(ev);
  }
}

void MemoryWrapper::clear()
{
  setSelected(getME());
  clearMe(getSelected());
  //mem_->clear();

  //doChange(getME(), EMemoryChange::mcClear);
}

void MemoryWrapper::clearMe(MEWrapper *me)
{
  clearMe1(me);
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
void MemoryWrapper::clearMe1(MEWrapper *me)
{
  if(me && me->getMe()) {
    doChange(me, EMemoryChange::mcClear);

    Memory::TME::Elements &childs = me->getMe()->getElements();
    int cnt = childs.count();
    for(int i = 0; i <cnt; ++i)
    {
      clearR(childs.get(i));
    }
    me->getMe()->clear();
  }
}

bool MemoryWrapper::move(MEWrapper *me, MEWrapper *parent, int pos)
{


  return move1(me, parent, pos);
}

bool MemoryWrapper::move1(MEWrapper *me, MEWrapper *parent, int pos)
{
  if(!me || !parent)
    return false;
  // запрещаем перенос из одного владельца в другого
  if(me->parent() != parent)
    return false;

  bool ok = me->getMe()->move_to(parent->getMe(), pos);
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
  foreach (auto me, map_mew_) {
    delete me;
  }

  map_mew_.clear();
}

MEWrapper *MemoryWrapper::add1(MEWrapper *parent, const QString &name, bool checkExist)
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
    me = CreateMEW(meParent->Add(name));
    if(me) {
      doChange(me, EMemoryChange::mcAdd);

//      Memory::TopME meEvent;
//      meEvent.setMem(this->mem_.get());
//      meEvent.Add("type")->setVal("Add");
//      meEvent.Add("me")->setVal(me->getPath());

//      emit change1(meEvent);
    }
  }

  return me;
}

QUndoStack *MemoryWrapper::getStack()
{
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





