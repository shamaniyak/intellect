#include "memorycontroller.h"

#include <QPlainTextEdit>

MemoryController::MemoryController(QObject *parent) : QObject(parent)
{

}

MemoryWrapper *MemoryController::mem() const
{
  return mem_;
}

void MemoryController::setMem(MemoryWrapper *mem)
{
  disconnectMem();
  mem_ = mem;
  if(mem_)
  {
    connectMem();
    memory_change(mem_->getSelected(), mcSelect);
  }
}

MEWrapper MemoryController::me()
{
  return sel_;
}

void MemoryController::setMe(const MEWrapper &me)
{
  checkForSave();
  sel_ = me;
  showVal();
}

IObject *MemoryController::iobj() const
{
  return iobj_;
}

void MemoryController::setIobj(IObject *iobj)
{
  iobj_ = iobj;
}

bool MemoryController::canChangeSelected() const
{
  return canChangeSelected_;
}

void MemoryController::setCanChangeSelected(bool canChangeSelected)
{
  canChangeSelected_ = canChangeSelected;
}

void MemoryController::timerEvent(QTimerEvent *)
{
  doSave();
}

void MemoryController::checkForSave()
{
  if(timerId_)
  {
    doSave();
  }
}

void MemoryController::doSave()
{
  this->killTimer(timerId_);
  timerId_ = 0;
  if(sel_)
  {
    disconnectMem();
    sel_.setVal(textEdit_->toPlainText());
    connectMem();
  }
}

void MemoryController::showVal()
{
  if(sel_)
    textEdit_->setPlainText(sel_.val().toString());
  else
    textEdit_->setPlainText("");
}

void MemoryController::memory_change(const MEWrapper &me, EMemoryChange idMsg)
{
  switch(idMsg)
  {
  case EMemoryChange::mcNone:
  case EMemoryChange::mcEditName:
  case EMemoryChange::mcAdd:
  case EMemoryChange::mcAddFrom:
  case EMemoryChange::mcUpdate:
  case mcMove:
  {
    break;
  }

  case EMemoryChange::mcSelect:
    if(me != sel_ && canChangeSelected_)
    {
      setMe(me);
    }
    break;

  case EMemoryChange::mcClear:
  case EMemoryChange::mcEditVal:
    if(me == sel_)
      showVal();
    break;

  case EMemoryChange::mcDel:
    if(me == sel_)
    {
      sel_ = MEWrapper();
      showVal();
    }
    break;

  default:
    break;
  }
}

void MemoryController::on_textChanged()
{
  //if(textEdit_->document()->isModified())
  {
    if(timerId_)
      this->killTimer(timerId_);
    timerId_ = this->startTimer(1000);
  }
}

QPlainTextEdit *MemoryController::textEdit() const
{
  return textEdit_;
}

void MemoryController::setTextEdit(QPlainTextEdit *textEdit)
{
  if(textEdit_)
    textEdit_->disconnect(this);
  textEdit_ = textEdit;
  if(textEdit_)
    connect(textEdit_, &QPlainTextEdit::textChanged, this, &MemoryController::on_textChanged);
}

void MemoryController::disconnectMem()
{
  if(mem_)
    mem_->disconnect(this);
}

void MemoryController::connectMem()
{
  if(mem_)
    connect(mem_, &MemoryWrapper::on_change, this, &MemoryController::memory_change);
}


