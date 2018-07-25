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

#include "iobject.h"
#include "objectmanager.h"
#include "windowmanager.h"

#include <src/Memory/qmemorymodel.h>

IObject::IObject(QObject *parent) : QObject(parent)
  ,scr_(std::make_shared<JSEngineWrapper>())
  ,qml_(new QQmlEngineWrapper(this))
{
  scr_->addObject(this, "IObj");
  qml_->addObject(this, "IObj");
  //scr_->addObject(mem_, "Memory");
}

IObject::~IObject()
{
  countLinks_ = 0;
}

IObject::t_jswrapper IObject::scr() const
{
  return scr_;
}

void IObject::release()
{
  --countLinks_;
  if(!countLinks_)
    deleteLater();
}

MemoryWrapper *IObject::mem()
{
  if(!mem_)
    mem_ = new QMemoryModel(this);
  return mem_;
}

bool IObject::run(const QString &path, const QVariant &/*params*/)
{
  msg_ = "";
  QString msg1 = QString("Object %1 run: %2").arg(objectName()).arg(path);
  qDebug() << msg1;
  log(msg1);

  if(objectName()=="INTELLECT")
    log(msg1);

  bool res = false;
  MEWrapper me = mem()->get(path);

  if(me) {
    res = execute( me.val().toString() );
  }
  else {
    msg_ = QString("Object %1 run %2: элемент памяти не найден.").arg(objectName()).arg(path);
    log(msg_);
  }

  return res;
}

void IObject::reset()
{
  scr()->clear();
}

bool IObject::execute(const QString &text)
{
  bool res = false;
  msg_ = QString("Object %1 execute: ").arg(objectName());

  if(!text.isEmpty()) {
    res = scr_->evaluate(text);

    msg_.append(scr_->msg()).append("; ");
  }
  else
    msg_.append("Скрипт не задан.");

  log(msg_);
  return res;
}

//QString IObject::msg() const
//{
//  return msg_;
//}

QObject *IObject::getObject(const QString &name)
{
  //ObjectManager *OM = dynamic_cast<ObjectManager*>(parent());

  //if(OM)
  //  return OM->Get(name);

  return( this->scr()->getObject(name) );

  //return nullptr;
}

void IObject::add_msg(const QString &msg)
{
  log(msg);
}

void IObject::log(const QString &msg)
{
  emit sendLog(msg);
}

WindowManager *IObject::getWM()
{
  if(!WM_)
    WM_ = new WindowManager(this);
  return WM_;
}

QObject *IObject::CM()
{
  if(!CM_)
    CM_ = new Commander(this);
  return CM_;
}

void IObject::sendQuery(const QString &receiver, const QString &path, const QString &params)
{
  // Если пусто, то запрос самому себе, иначе объекту с именем receiver
  if(receiver.isEmpty())
  {
    emit signalQuery(path, params);
  }
  else
  {
    auto obj = qobject_cast<IObject*>( getObject(receiver) );
    if(obj)
      obj->sendQuery("", path, params);
  }
}

void IObject::onQuery(const QString &path, const QString &params)
{
  run(path, params);
}

QQmlEngineWrapper *IObject::getQml() const
{
  return qml_;
}

QObject *IObject::getOM()
{
  if(!OM_)
    OM_ = new ObjectManager(this);
  return OM_;
}

int IObject::countLinks() const
{
  return countLinks_;
}

void IObject::incCountLinks()
{
  ++countLinks_;
}

bool IObject::runQML(const QString &text)
{
  msg_ = "";
  bool result = false;
  if(qml_)
  {
    result = qml_->evaluate(text);
    msg_ = QString("Object %1 runQML: %2").arg(objectName()).arg(qml_->msg());
    log(msg_);
  }

  return result;
}

bool IObject::addObject(QObject *obj, const QString &name)
{
  bool isAdd = this->scr()->addObject(obj, name);
  if(isAdd)
    qml_->addObject(obj, name);
  return isAdd;
}
