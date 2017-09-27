#include <QApplication>
#include <QMainWindow>
#include <QKeyEvent>

#include "intellect.h"
#include "windowmanager.h"

Intellect::Intellect(QObject *parent):
  TAlgorithm(parent)
{
  QString fname = QApplication::applicationDirPath() + "/globalMemory.moi";
  getMM()->globalMemory()->open(fname);

  obj_ = getObject("Intellect");
  //obj_->mem()->setAutosave(true);

  obj_->addObject(this, "II");

  // Открыть файл памяти
  fname = QApplication::applicationDirPath() + "/memory.moi";
  obj_->mem()->open(fname);

  // Добавить элементы памяти
  auto me = obj_->mem()->add(nullptr, "Intellect")->add("Create");
  if(me->val().toString().isEmpty())
    me->setVal("\"intellect version 3.0\"");

  auto val = obj_->mem()->getME()->val();
  QString newVal = "intellect ver. 3.0.0.0 " + QString(QChar(0xE3)) + "Shamaniyak ";
  if(val != newVal)
    obj_->mem()->getME()->setVal(newVal);

  connect(getOM(), &ObjectManager::signalAddObject, this, &Intellect::on_addObject);
  connect(this, &TAlgorithm::start, this, &Intellect::on_start);
}

Intellect::~Intellect()
{
  disconnect(getOM(), &ObjectManager::signalAddObject, this, &Intellect::on_addObject);
  disconnect(this, &TAlgorithm::start, this, &Intellect::on_start);

  //QString fname = QApplication::applicationDirPath() + "/globalMemory.moi";
  getMM()->globalMemory()->clear();
  //MM()->globalMemory()->setFilePath(fname);
  getMM()->globalMemory()->save();
}

IObject* Intellect::obj() const
{
  return obj_;
}

QMainWindow *Intellect::getMainWindow() const
{
  return mainWindow_;
}

void Intellect::setMainWindow(QMainWindow *mw)
{
  mainWindow_ = mw;
  getWM()->setMain_window(mw);
}

QString Intellect::compileText(const QString &str)
{
  IObject *iobj = getObject("Compiler");

  if(iobj) {
    iobj->addObject(this, "II");
    iobj->execute(str);

    return "";
  }
  else
    return tr("Compiler not create");

//  ScriptWrapper script;
//  script.setText(ui->txtVal->toPlainText());
//  script.execute();
//  AddResult(script.msg());

  return "";
}

int Intellect::loadPlugins()
{
  getPM()->loadPlugins();

  return getPM()->count();
}

void Intellect::event(QObject *obj, QEvent *ev)
{
  if( ev->type() == QEvent::KeyPress )
  {
    keyEvent(obj, ev);
  }

//  bool isFocusEvent = ev->type() == QEvent::FocusIn || ev->type() == QEvent::FocusOut;
//  if(isFocusEvent)
//  {
//    focusEvent(obj, ev);
  //  }
}

void Intellect::stop()
{
  this->getOM()->abort();
}

void Intellect::on_start()
{
  disconnect(this, &TAlgorithm::start, this, &Intellect::on_start);

  loadPlugins();
  obj()->run("Intellect\\Create");
}

bool Intellect::keyEvent(QObject *obj, QEvent *ev)
{
  if(!obj)
    return false;
  QKeyEvent *kev = dynamic_cast<QKeyEvent *>(ev);

  if(kev) {
    bool ctrl_pressed = kev->modifiers() & Qt::ControlModifier;

    switch(kev->key())
    {

    case Qt::Key_S:
    {
      if(ctrl_pressed)
      {
      }
    }break;

    case Qt::Key_Return:
    {

    }break;
    }
  }

  IObject *o = qobject_cast<IObject*>( getOM()->Get("KeyManager") );
  if(o)
  {
    o->mem()->add(nullptr, "key")->setVal(kev->key());
  }

  return false;
}

QObject *Intellect::getMainWindowQObject() const
{
  return mainWindow_;
}

void Intellect::on_addObject(QObject *obj)
{
  IObject *iobj = qobject_cast<IObject*>(obj);
  if(iobj)
    iobj->addObject(this, "II");
}
