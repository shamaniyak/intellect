#include "highlighter.h"
#include "scripteditor.h"

#include <intellect.h>

#include <QMessageBox>

ScriptEditor::ScriptEditor(QWidget *parent) : QPlainTextEdit(parent),
  h_(new Highlighter(this->document()))
{
  this->setObjectName("ScriptEditor");

  QFont font;
  font.setFamily("Courier");
  font.setFixedPitch(true);
  font.setPointSize(10);

  this->setFont(font);

  this->setLineWrapMode(QPlainTextEdit::NoWrap);

  connect(this, &ScriptEditor::textChanged, this, &ScriptEditor::on_textChanged);
}

MemoryWrapper *ScriptEditor::mem() const
{
  return mem_;
}

void ScriptEditor::disconnectMem()
{
  if(mem_)
    mem_->disconnect(this);
}

void ScriptEditor::connectMem()
{
  if(mem_)
    connect(mem_, &MemoryWrapper::on_change, this, &ScriptEditor::memory_change);
}

void ScriptEditor::setMem(MemoryWrapper *mem)
{
  disconnectMem();
  mem_ = mem;
  connectMem();
  if(mem_)
  {
    memory_change(mem_->selected(), mcSelect);
  }
}

void ScriptEditor::timerEvent(QTimerEvent *)
{
  save();
}

void ScriptEditor::keyPressEvent(QKeyEvent *kev)
{
  bool ctrl_pressed = kev->modifiers() & Qt::ControlModifier;
  bool shift_pressed = kev->modifiers() & Qt::ShiftModifier;
  bool alt_pressed = kev->modifiers() & Qt::AltModifier;

  if(intellect_)
  {
    IObject *editor = qobject_cast<IObject*>( intellect_->GetObject("Editor") );
    if(editor)
    {
      auto me = editor->mem()->add(0, "KeyEvent");
      me->clear();

      if(ctrl_pressed)
        me->add("Ctrl");
      if(shift_pressed)
        me->add("Shift");
      if(alt_pressed)
        me->add("Alt");

      me->add("Key")->setVal(kev->key());
      me->add("Text")->setVal(kev->text());
    }
  }

  emit signalKeyPress(this, kev->key());

  switch(kev->key())
  {
    case Qt::Key_Return:
    {
      //doReturn();
      //kev->ignore();
      break;
    }
  }

  QPlainTextEdit::keyPressEvent(kev);
}

void ScriptEditor::checkForSave()
{
  if(timerId_)
  {
//    int n = QMessageBox::warning(0,
//                                 "Редактор",
//                                 "Значение изменено. Сохранить?",
//                                 QMessageBox::Yes | QMessageBox::No,
//                                 QMessageBox::Yes);

//    if(n == QMessageBox::Yes)
      save();
  }
}

void ScriptEditor::save()
{
  this->killTimer(timerId_);
  timerId_ = 0;

  if(sel_)
  {
    disconnectMem();
    sel_->setVal(this->toPlainText());
    connectMem();
  }
}

void ScriptEditor::doReturn()
{
  //QString str = this->toPlainText();
  //QStringList strList = str.split('\n');
  //QTextCharFormat format;
  //format.setVerticalAlignment(QTextCharFormat::AlignBaseline);
  //this->textCursor().setBlockCharFormat(format);
  //this->textCursor().insertBlock();

  int pos = this->textCursor().position();
  textCursor().movePosition(QTextCursor::Left);
  //this->textCursor().insertText("\n ");
}

void ScriptEditor::memory_change(MEWrapper *me, EMemoryChange idMsg)
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
      if(me != sel_)
      {
        checkForSave();
        sel_ = me;
        showVal();
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
        sel_ = 0;
        showVal();
      }
      break;

    default:
      break;
  }
}

void ScriptEditor::on_textChanged()
{
  if(this->document()->isModified())
  {
    if(timerId_)
      this->killTimer(timerId_);
    timerId_ = this->startTimer(1000);
  }
}

Intellect *ScriptEditor::intellect() const
{
  return intellect_;
}

void ScriptEditor::setIntellect(Intellect *intellect)
{
  intellect_ = intellect;
}

void ScriptEditor::showVal()
{
  this->blockSignals(true);
  if(sel_)
  {
    this->setPlainText(sel_->val().toString());
  }
  else
  {
    this->setPlainText("");
  }
  this->blockSignals(false);
}
