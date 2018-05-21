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

#include "memoryeditor.h"

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
  AddCommand(MemoryWrapper *m, MEWrapper &parent, const QString &name, bool checkExist) :
    BaseCommand(m, "Add"),
    parent_(parent), name_(name), checkExist_(checkExist) {   }

  virtual void undo() override
  {
    m_->deleteMe(newMe_);
  }
  virtual void redo() override
  {
    newMe_ = m_->add(parent_, name_, checkExist_);
  }

  MEWrapper newMe() const { return newMe_; }

private:
  MEWrapper parent_;
  QString name_;
  bool checkExist_ = true;
  MEWrapper newMe_;
};

class AddFromCommand : public BaseCommand
{
public:
  AddFromCommand(MemoryWrapper *m, MEWrapper &parent, MEWrapper &from, bool recurs) : BaseCommand(m, "AddFrom"),
    parent_(parent), from_(from), recurs_(recurs)
  {
    beginIndex_ = parent_.count();
  }

  virtual void undo() override
  {
    for(int i = parent_.count() -1; i >= beginIndex_; --i)
      parent_.delByI(i);
  }
  virtual void redo() override
  {
    m_->addFrom(parent_, from_, recurs_);
  }

private:
  MEWrapper parent_;
  MEWrapper from_;
  bool recurs_ = true;
  int beginIndex_ = 0;
};

class DelCommand : public BaseCommand
{
public:
  DelCommand(MemoryWrapper *m, MEWrapper &me) : BaseCommand(m, "Del"),
    me_(me), parent_(me.parent()), name_(me.name()), index_(me.getIndex())
  {
    buf_.setMem(m->mem_.get());
    buf_.addFrom(me_.getMe(), true);
  }

  virtual void undo() override
  {
    me_ = m_->add(parent_, name_, false);
    m_->move(me_, parent_, index_);
    m_->addFrom(me_, MEWrapper(&buf_), true);
  }
  virtual void redo() override
  {
    m_->deleteMe(me_);
  }

private:
  MEWrapper me_;
  MEWrapper parent_;
  QString name_;
  int index_;
  Memory::TopME buf_;
};

class EditNameCommand : public BaseCommand
{
public:
  EditNameCommand(MemoryWrapper *m, MEWrapper &me, const QString &name) :
    BaseCommand(m, "EditName"), me_(me), newName_(name), oldName_(me.name())
  {  }

  virtual void undo() override
  {
    m_->setName(me_, oldName_);
  }
  virtual void redo() override
  {
    m_->setName(me_, newName_);
  }

private:
  MEWrapper me_;
  QString newName_, oldName_;
};

class EditValCommand : public BaseCommand
{
public:
  EditValCommand(MemoryWrapper *m, MEWrapper &me, const QVariant &val) :
    BaseCommand(m, "EditVal"), me_(me), newVal_(val), oldVal_(me.val())
  {  }

  virtual void undo() override
  {
    m_->setVal(me_, oldVal_);
  }
  virtual void redo() override
  {
    m_->setVal(me_, newVal_);
  }

private:
  MEWrapper me_;
  QVariant newVal_, oldVal_;
};

class ClearCommand : public BaseCommand
{
public:
  ClearCommand(MemoryWrapper *m, const MEWrapper &me) : BaseCommand(m, "Clear"),
    me_(me)
  {
    buf_.setMem(m->mem_.get());
    buf_.addFrom(me_.getMe(), true);
  }

  virtual void undo() override
  {
    m_->addFrom(me_, MEWrapper(&buf_), true);
  }
  virtual void redo() override
  {
    m_->clearMe(me_);
  }

private:
  MEWrapper me_;
  Memory::TopME buf_;
};

class MoveCommand : public BaseCommand
{
public:
  MoveCommand(MemoryWrapper *m, MEWrapper me, int index) : BaseCommand(m, "Move"),
    me_(me), newIndex_(index), oldIndex_(me.getIndex())
  {}

  virtual void undo() override
  {
    m_->move(me_, me_.parent(), oldIndex_);
  }
  virtual void redo() override
  {
    m_->move(me_, me_.parent(), newIndex_);
  }

private:
  MEWrapper me_;
  int newIndex_, oldIndex_;
};


MemoryEditor::MemoryEditor(QObject *parent) : QObject(parent)
{
  stack_ = new QUndoStack(this);
}

void MemoryEditor::add(MEWrapper &parent, const QString &name, bool checkExist)
{
  if(!mem_)
    return;
  if(!parent)
    parent = mem_->getME();
  if(parent)
  {
    AddCommand *cmd = new AddCommand(mem_, parent, name, checkExist);
    stack_->push(cmd);
  }
}

void MemoryEditor::addFrom(MEWrapper &parent, MEWrapper &mefrom, bool recurs)
{
  if(!mem_) return;
  if(!parent)
    parent = mem_->getME();

  if(parent && mefrom)
  {
    auto cmd = new AddFromCommand(mem_, parent, mefrom, recurs);
    stack_->push(cmd);
  }
}

void MemoryEditor::del(const QString &path)
{
  if(!mem_) return;
  auto me = mem_->get(path);

  if(!me || !me.parent())
    return;

  deleteMe(me);
}

void MemoryEditor::deleteMe(MEWrapper &me)
{
  if(mem_ && me)
  {
    DelCommand *cmd = new DelCommand(mem_, me);
    stack_->push(cmd);
  }
}

void MemoryEditor::setName(MEWrapper &me, const QString &name)
{
  if(mem_ && me)
  {
    if(name == me.name())
      return;

    auto cmd = new EditNameCommand(mem_, me, name);
    stack_->push(cmd);
  }
}

void MemoryEditor::setVal(MEWrapper &me, const QVariant &val)
{
  if(mem_ && me)
  {
    if(val == me.val())
      return;

    auto cmd = new EditValCommand(mem_, me, val);
    stack_->push(cmd);
  }
}

void MemoryEditor::clear()
{
  if(mem_)
    clearMe(mem_->getME());
}

void MemoryEditor::clearMe(const MEWrapper &me)
{
  if(mem_ && me) {
    auto cmd = new ClearCommand(mem_, me);
    stack_->push(cmd);
  }
}

void MemoryEditor::move(MEWrapper &me, MEWrapper &parent, int pos)
{
  if(mem_ && me && parent) {
    auto cmd = new MoveCommand(mem_, me, pos);
    stack_->push(cmd);
  }
}

MemoryWrapper *MemoryEditor::getMem() const
{
  return mem_;
}

void MemoryEditor::setMem(MemoryWrapper *mem)
{
  mem_ = mem;
}
