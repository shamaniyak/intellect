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

#include <QApplication>
#include <QMainWindow>
#include <QKeyEvent>

#include "intellect.h"
#include <Src/windowmanager.h>

Intellect::Intellect(QObject *parent):
  TAlgorithm(parent)
{
  QString fname = QApplication::applicationDirPath() + "/globalMemory.moi";
  getMM()->globalMemory()->open(fname);

  obj_ = getObject("Intellect");
  connect(obj_, &IObject::sendLog, this, &Intellect::log);
  //obj_->mem()->setAutosave(true);
  obj_->addObject(this, "II");

  // Открыть файл памяти
  fname = QApplication::applicationDirPath() + "/IntellectMemory.moi";
  obj_->mem()->open(fname);

  // Добавить элементы памяти
  auto me = obj_->mem()->add(MEWrapper(), "Intellect").add("Create");
  if(me && me.val().toString().isEmpty())
    me.setVal("\"intellect version 5.0\"");

  me = obj_->mem()->getME();
  if(me) {
    auto val = me.val();
    QString newVal = "intellect 5.0.0.0\n(c) 2010-2018 Shamaniyak";
    if(val != newVal)
      me.setVal(newVal);
  }

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

QString Intellect::applicationDirPath()
{
  return QApplication::applicationDirPath();
}

bool Intellect::putEnv(const QString &name, const QVariant &val)
{
  return(qputenv(name.toStdString().data(), val.toByteArray())!=0);
}

QVariant Intellect::getEnv(const QString &name)
{
  auto barr = qgetenv(name.toStdString().data());
  return QVariant(barr);
}

ProcessEnvironment *Intellect::processEnvironment()
{
    return new ProcessEnvironment();
}

QString Intellect::urlToNativeFilePath(const QUrl &filePath)
{
    auto fName = filePath.toLocalFile();
    if(filePath.isLocalFile()) {
      fName = QDir::toNativeSeparators(fName);
      //qDebug() << fName;
    }
    return fName;
}

void Intellect::on_start()
{
  disconnect(this, &TAlgorithm::start, this, &Intellect::on_start);

  //loadPlugins();
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
    o->mem()->add(MEWrapper(), "key").setVal(kev->key());
  }

  return false;
}

QObject *Intellect::getMainWindowQObject() const
{
  return mainWindow_;
}

void Intellect::recreateMemory()
{
//  Memory::TMemory tempMemory, tempMemory1;
//  QDir dir(QApplication::applicationDirPath()+"/bkp");
//  if(dir.exists())
//  {
//    auto dirsInfoList = dir.entryInfoList(QDir::Dirs);
//    for(auto &&dirInfo: dirsInfoList)
//    {
//      if(!dirInfo.isDir())
//        continue;
//      if(dirInfo.fileName() == "." || dirInfo.fileName() == "..")
//        continue;
//      QDir dir1(dirInfo.filePath());
//      auto filesInfoList = dir1.entryInfoList(QDir::Files);
//      for(auto &&fileInfo: filesInfoList)
//      {
//        tempMemory1.clear();
//        if(tempMemory.open(fileInfo.filePath()))
//        {
//          tempMemory1.getTopME()->setVal(tempMemory.getTopME()->val());
//          tempMemory1.addFrom(0, tempMemory.getTopME(), true);
//          tempMemory1.setFilePath(fileInfo.filePath());
//          if(!tempMemory1.save())
//          {
//            qDebug() << fileInfo.filePath() << " not save." << "\n";
//          }
//        }
//      }
//    }
//  }
}

void Intellect::on_addObject(QObject *obj)
{
  IObject *iobj = qobject_cast<IObject*>(obj);
  if(iobj) {
    iobj->addObject(this, "II");
    connect(iobj, &IObject::sendLog, this, &Intellect::log);
    //connect(iobj, SIGNAL(sendLog(QString)), this, SIGNAL(log(QString)));
  }
}
