﻿/****************************************************************************
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
#include "talgorithm.h"
#include "windowmanager.h"

#include <src/Memory/qmemorymodel.h>

IObject::IObject(QObject *parent) : QObject(parent)
  ,scr_(new JSEngineWrapper)
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
  msg_ = "Object " + objectName() + " run " + path;
  add_msg(msg_);

  bool res = false;
  MEWrapper me = mem()->get(path);

  if(me) {
    res = execute( me.val().toString() );
  }
  else {
    msg_ = QString("Object ") + objectName() + ": Элемент памяти не найден.";
    add_msg(msg_);
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

  if(!text.isEmpty()) {
    res = scr_->evaluate(text);

    add_msg("execute = " + scr_->msg());
  }
  else
    add_msg("Скрипт не задан.");

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
  TAlgorithm *A = qobject_cast<TAlgorithm*>(parent()->parent());
  if(A)
    A->addResult(msg);
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
  msg_ = "Object " + objectName() + " runQML:";
  add_msg(msg_);

  if(qml_)
  {
    bool is_exec = qml_->evaluate(text);
    add_msg(qml_->msg());

    return is_exec;
  }
  return false;
}

bool IObject::addObject(QObject *obj, const QString &name)
{
  bool isAdd = this->scr()->addObject(obj, name);
  if(isAdd)
    qml_->addObject(obj, name);
  return isAdd;
}
