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

#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <QObject>
#include <QMap>

class IObject;

class ObjectManager : public QObject
{
  Q_OBJECT
public:
  explicit ObjectManager(QObject *parent = 0);

  void abort();

signals:
  void signalAddObject(QObject *obj);

public slots:
  QObject *Add(const QString &name);
  QObject *Get(const QString &name);
  bool Del(const QString &name);

protected:
  void CreateObject(const QString &name);

private:
  typedef QMap<QString, IObject*> t_objects;
  t_objects objects_;
};

#endif // OBJECTMANAGER_H
