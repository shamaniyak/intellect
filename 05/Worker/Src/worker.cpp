#include <QApplication>
#include <QMainWindow>
#include <QKeyEvent>

#include "worker.h"
#include <Src/windowmanager.h>

Worker::Worker(QObject *parent):
  TAlgorithm(parent)
{
  QString fname = QApplication::applicationDirPath() + "/globalMemory.moi";
  getMM()->globalMemory()->open(fname);

  obj_ = getObject("Core");
  //obj_->mem()->setAutosave(true);

  obj_->addObject(this, "core");

  // Открыть файл памяти
  fname = QApplication::applicationDirPath() + "/memory.moi";
  obj_->mem()->open(fname);

  // Добавить элементы памяти
  auto me = obj_->mem()->add(nullptr, "Main");
  if(me->val().toString().isEmpty())
    me->setVal("worker version 1.0");

  auto val = obj_->mem()->getME()->val();
  QString newVal = "worker 1.0.0.0 " + QString(QChar(0xE3)) + "Shamaniyak ";
  if(val != newVal)
    obj_->mem()->getME()->setVal(newVal);

  connect(getOM(), &ObjectManager::signalAddObject, this, &Worker::on_addObject);
  connect(this, &TAlgorithm::start, this, &Worker::on_start);
}

Worker::~Worker()
{
  disconnect(getOM(), &ObjectManager::signalAddObject, this, &Worker::on_addObject);
  disconnect(this, &TAlgorithm::start, this, &Worker::on_start);

  //QString fname = QApplication::applicationDirPath() + "/globalMemory.moi";
  getMM()->globalMemory()->clear();
  //MM()->globalMemory()->setFilePath(fname);
  getMM()->globalMemory()->save();
}

IObject* Worker::obj() const
{
  return obj_;
}

QMainWindow *Worker::getMainWindow() const
{
  return mainWindow_;
}

void Worker::setMainWindow(QMainWindow *mw)
{
  mainWindow_ = mw;
  getWM()->setMain_window(mw);
}

QString Worker::compileText(const QString &str)
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

int Worker::loadPlugins()
{
  getPM()->loadPlugins();

  return getPM()->count();
}

void Worker::event(QObject *obj, QEvent *ev)
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

void Worker::stop()
{
  this->getOM()->abort();
}

void Worker::on_start()
{
  disconnect(this, &TAlgorithm::start, this, &Worker::on_start);

  loadPlugins();
  obj()->run("Intellect\\Create");
}

bool Worker::keyEvent(QObject *obj, QEvent *ev)
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

QObject *Worker::getMainWindowQObject() const
{
  return mainWindow_;
}

void Worker::on_addObject(QObject *obj)
{
  IObject *iobj = qobject_cast<IObject*>(obj);
  if(iobj)
    iobj->addObject(this, "II");
}
