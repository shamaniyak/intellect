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

#ifndef TALGORITHM_H
#define TALGORITHM_H

#include <QObject>
//#include "tmemory.h"
#include "Memory/memorymanager.h"
#include "objectmanager.h"
#include "pluginmanager.h"
#include "windowmanager.h"
#include "QmlManager/qmlmanager.h"

// ���������� ���������
class TAlgorithm : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QObject* MM READ getMM)
  Q_PROPERTY(QObject* OM READ getOM)
  Q_PROPERTY(QObject* PM READ getPM)
  Q_PROPERTY(WindowManager* WM READ getWM)
  Q_PROPERTY(QObject* Qml READ getQml)

public:
  explicit TAlgorithm(QObject *parent = 0);
  ~TAlgorithm();

  // �������� ������� ������.
  // ��� ���������� ������� ������ IObject � ������ name
  //  � ��������� ������ obj � ������ � ������ name.
  // ���� name �� ������, �� � �������� ����� ����� QObject::objectName().
  // ������ false, ���� ��� �� ������ ��� ����������, ����� true.
  bool addObject(QObject *obj, const QString &name);

  ObjectManager *getOM() const;

  PluginManager *getPM() const;

  MemoryManager *getMM() const;

  WindowManager* getWM() const;

  IObject *getObject(const QString &name);

  QmlManager *getQml() const;

signals:
  void start();

public slots:

protected slots:

private:
  MemoryManager *MM_ = 0;
  ObjectManager *OM_ = 0;// �������� ��������
  PluginManager *PM_ = 0;  // �������� ��������
  WindowManager* WM_ = 0;
  QmlManager* qml_ = 0;   // �������� QML
};

#endif // TALGORITHM_H
