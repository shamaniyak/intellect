#include "dialogmemoryeditor.h"
#include "ui_dialogmemoryeditor.h"

#include <QMenuBar>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSettings>

#include "intellect.h"
#include "Memory/memorywrapper.h"
#include "scriptwrapper.h"

#include "memorytreeview.h"

DialogMemoryEditor::DialogMemoryEditor(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Editor)
{
  ui->setupUi(this);

  Init();

  CreateMainMenu();

  //installEventFilter(this);
}

DialogMemoryEditor::~DialogMemoryEditor()
{
  if(intellect_)
  {
    // удалим свои объекты скриптов
    intellect_->OM()->Del("Editor");

    disconnect(intellect_, &TAlgorithm::addResult, this, &DialogMemoryEditor::on_addResult);
    disconnect(intellect_->obj()->mem(), &MemoryWrapper::on_change, this, &DialogMemoryEditor::memory_change);
  }

  delete ui;
}

void DialogMemoryEditor::Init()
{
  ui->edtName->installEventFilter(this);
  ui->txtVal->installEventFilter(this);

  ui->btnEdit->setEnabled(false);
}

void DialogMemoryEditor::CreateMainMenu()
{
  QMenuBar *mnuBar = new QMenuBar(ui->groupBox_5);
  QMenu *pmenu = new QMenu("&Menu");

  actSave = pmenu->addAction("&Save");
  connect(actSave, &QAction::triggered, this, &DialogMemoryEditor::actSave_triggered);

  actAbout = pmenu->addAction("&About QT");

  mnuBar->addMenu(pmenu);

  pmenu = new QMenu("&Edit");
  actUndo = pmenu->addAction("Undo Backup");
  connect(actUndo, &QAction::triggered, this, &DialogMemoryEditor::edit_undo_triggered);

  actRedo = pmenu->addAction("Redo Backup");
  connect(actRedo, &QAction::triggered, this, &DialogMemoryEditor::edit_redo_triggered);

  mnuBar->addMenu(pmenu);

  mnuBar->show();
}

void DialogMemoryEditor::SetChanged(bool val)
{
  selected_changed_ = val;

  ui->btnEdit->setEnabled(val);
}

Intellect *DialogMemoryEditor::intellect() const
{
  return intellect_;
}

void DialogMemoryEditor::setIntellect(Intellect *intellect)
{
  intellect_ = intellect;

  if(intellect_)
  {
    intellect_->addObject(this, "Editor");

    connect(intellect_, &TAlgorithm::addResult, this, &DialogMemoryEditor::on_addResult);
    connect(intellect_->obj()->mem(), &MemoryWrapper::on_change, this, &DialogMemoryEditor::memory_change);
  }

  ShowMemory();

  SetupEditor();

  UpdateUI();
}

void DialogMemoryEditor::checkItemChanged(QObject *obj)
{
  if(selected_changed_)
  {
    if(obj == ui->edtName)
      SetChanged(false);
    else if(obj == ui->txtVal)
    {
      int n = QMessageBox::warning(0,
                                   "Редактор",
                                   "Значение изменено. Сохранить?",
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::Yes);

      if(n == QMessageBox::Yes)
        SaveChanges(ui->txtVal);
      else
        SetChanged(false);
    }
  }
}

void DialogMemoryEditor::LoadSettings(MEWrapper *meSettings)
{
  if(meSettings)
  {
    auto w = meSettings->get("Width")->val().toInt();
    auto h = meSettings->get("Height")->val().toInt();
    baseSize_ = QSize(w, h);
    this->resize(baseSize_);

    QList<int> sizes;

    auto meGrpVal = meSettings->get("grpVal");

    if(meGrpVal)
    {
      w = meGrpVal->get("Width")->val().toInt();
      h = meGrpVal->get("Height")->val().toInt();

      sizes << h;
    }

    auto meGrpResult = meSettings->get("grpResult");

    if(meGrpResult)
    {
      w = meGrpResult->get("Width")->val().toInt();
      h = meGrpResult->get("Height")->val().toInt();

      sizes << h;
    }

    ui->splitter->setSizes(sizes);
  }


}

void DialogMemoryEditor::SaveSettings(MEWrapper *meSettings)
{
  if(!intellect_)
    return;

  if(meSettings)
  {
    // ширина и высота диалогового окна
    meSettings->add("Width")->setVal(this->size().width());
    meSettings->add("Height")->setVal(this->size().height());
    // размеры редактора значения и панели результата
    meSettings->add("grpVal")->add("Width")->setVal(ui->grpVal->size().width());
    meSettings->add("grpVal")->add("Height")->setVal(ui->grpVal->size().height());
    meSettings->add("grpResult")->add("Width")->setVal(ui->grpResult->size().width());
    meSettings->add("grpResult")->add("Height")->setVal(ui->grpResult->size().height());
  }
}

QSize DialogMemoryEditor::sizeHint() const
{
  //QSize sz = size();
  return baseSize_;
}

void DialogMemoryEditor::SaveChanges(QObject *obj)
{
  if(selected_)
  {
    if(obj == ui->edtName)
      selected_->setName(ui->edtName->text());
    else if(obj == ui->txtVal)
      selected_->setVal(ui->txtVal->toPlainText());
  }

  SetChanged(false);
}

bool DialogMemoryEditor::eventFilter(QObject *obj, QEvent *ev)
{
  if( ev->type() == QEvent::KeyPress )
  {
    if(keyEvent(obj, ev))
      return true;
  }

  bool isFocusEvent = ev->type() == QEvent::FocusIn || ev->type() == QEvent::FocusOut;
  if(isFocusEvent)
  {
    focusEvent(obj, ev);
  }

  return QWidget::eventFilter(obj, ev);
}

bool DialogMemoryEditor::focusEvent(QObject *obj, QEvent *ev)
{
  QFocusEvent *fev = static_cast<QFocusEvent*>(ev);
  if(fev->gotFocus())
    focussed_object_ = obj;

  //checkItemChanged(obj);

  return false;
}

bool DialogMemoryEditor::keyEvent(QObject *obj, QEvent *ev)
{
  QKeyEvent *kev = dynamic_cast<QKeyEvent *>(ev);
  bool res = false;

  if(kev) {
    bool ctrl_pressed = kev->modifiers() & Qt::ControlModifier;
    bool shift_pressed = kev->modifiers() & Qt::ShiftModifier;
    bool alt_pressed = kev->modifiers() & Qt::AltModifier;

    switch(kev->key())
    {

      case Qt::Key_S:
      {
        if(ctrl_pressed)
        {
          bool can_save = obj == ui->edtName || obj == ui->txtVal;
          if(can_save)
          {
            SaveChanges(obj);
            ev->accept();
            res = true;
          }
        }
      }break;

      case Qt::Key_Return:
      {
        if(obj ==ui->edtName)
          on_btnAdd_clicked();
      }break;
    }

    IObject *editor = qobject_cast<IObject*>( intellect()->GetObject("Editor") );
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

//      if(kev->key() != Qt::Key_Control
//         && kev->key() != Qt::Key_Shift
//         && kev->key() != Qt::Key_Alt
//         )
        me->add("Key")->setVal(kev->key());
        me->add("Text")->setVal(kev->text());
    }

    emit signalKeyPress(obj, kev->key());
  }

  intellect()->event(obj, ev);

  return res;
}

void DialogMemoryEditor::ShowMemory()
{
  if(!intellect_)
    return;

  memory_change(intellect_->obj()->mem()->selected(), mcSelect);

  //ShowMeValues(intellect_->obj()->mem()->selected());
}

void DialogMemoryEditor::SetupEditor()
{
  QFont font;
  font.setFamily("Courier");
  font.setFixedPitch(true);
  font.setPointSize(10);

  ui->txtVal->setFont(font);

  highlighter_ = new Highlighter(ui->txtVal->document());
}

void DialogMemoryEditor::ShowTxtVal(const QString &val)
{
  ui->txtVal->blockSignals(true);
  ui->txtVal->setPlainText(val);
  ui->txtVal->blockSignals(false);
}

void DialogMemoryEditor::ShowMeValues(MEWrapper *me)
{
  if(me)
  {
    ui->edtName->setText(me->name());
    ui->edtPath->setText(me->getPath());

    ShowTxtVal(me->val().toString());
  }
  else
    ClearValues();
}

void DialogMemoryEditor::AddResult(const QString &str)
{
  if(!str.isEmpty())
    ui->txtResult->appendPlainText(str);
}

void DialogMemoryEditor::ClearValues()
{
  ui->edtName->clear();
  ui->edtPath->clear();

  ShowTxtVal("");
}

void DialogMemoryEditor::UpdateUI()
{
  ui->btnDelete->setEnabled(selected_);

  if(intellect_)
  {
    actUndo->setEnabled(intellect_->obj()->mem()->canUndoBackup());
    actRedo->setEnabled(intellect_->obj()->mem()->canRedoBackup());
  }
}

void DialogMemoryEditor::on_btnAdd_clicked()
{
  if(!intellect_)
    return;

  QString name = ui->edtName->text();
  MEWrapper *me = selected_->get(name);
  if(me)
    intellect_->obj()->mem()->setSelected(me);
  else
    me = intellect_->obj()->mem()->add(selected_, name);

  if(me) {
    ui->btnEdit->setEnabled(false);
  }
}

void DialogMemoryEditor::on_btnEdit_clicked()
{
  SaveChanges(ui->txtVal);
}

void DialogMemoryEditor::on_btnDelete_clicked()
{
  MEWrapper *parent = selected_ ? selected_->parent() : 0;

  if( parent )
  {
    parent->del(selected_->name());
  }
}

void DialogMemoryEditor::on_btnCompile_clicked()
{
  if(!intellect_)
    return;

  const QString &str = ui->txtVal->toPlainText();
  AddResult(intellect_->compileText(str));
}

void DialogMemoryEditor::memory_change(MEWrapper *me, EMemoryChange idMsg)
{
  switch(idMsg)
  {
    case EMemoryChange::mcNone:
      break;
    case EMemoryChange::mcSelect:
    {
      if(selected_changed_)
        checkItemChanged(focussed_object_);
      selected_ = me;
      ShowMeValues(me);
      //UpdateUI();
      break;
    }
    case EMemoryChange::mcAdd:
    {
      break;
    }
    case EMemoryChange::mcClear:
      break;
    case EMemoryChange::mcDel:
    {
      if(me == selected_ && selected_changed_)
        checkItemChanged(focussed_object_);
      break;
    }
    case EMemoryChange::mcEditName:
    case EMemoryChange::mcEditVal:
      if(me == selected_ && !selected_changed_)
        ShowMeValues(me);
      break;
    case EMemoryChange::mcAddFrom:
    case EMemoryChange::mcUpdate:
      break;
    case mcMove:
      break;
  }

  UpdateUI();
}

void DialogMemoryEditor::on_txtVal_textChanged()
{
  if(ui->txtVal->document()->isModified())
    SetChanged();
}

void DialogMemoryEditor::on_addResult(const QString &str)
{
  AddResult(str);
}

void DialogMemoryEditor::on_edtName_textEdited(const QString &)
{
  SetChanged();
}

void DialogMemoryEditor::on_txtVal_modificationChanged(bool )
{
//  if(arg1)
  //    SetChanged();
}

void DialogMemoryEditor::edit_undo_triggered(bool )
{
  if(!intellect_)
    return;

  intellect_->obj()->mem()->undoBackup();

  ShowMemory();
  ShowMeValues(intellect_->obj()->mem()->selected());
}

void DialogMemoryEditor::edit_redo_triggered(bool )
{
  if(!intellect_)
    return;

  intellect_->obj()->mem()->redoBackup();

  ShowMemory();
  ShowMeValues(intellect_->obj()->mem()->selected());
}

void DialogMemoryEditor::actSave_triggered(bool /*checked*/)
{
  intellect_->obj()->mem()->save();
}

QAction *DialogMemoryEditor::getActSave() const
{
  return actSave;
}

QAction *DialogMemoryEditor::getActAbout() const
{
  return actAbout;
}

QAction *DialogMemoryEditor::getActRedo() const
{
  return actRedo;
}

QAction *DialogMemoryEditor::getActUndo() const
{
  return actUndo;
}
