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

#ifndef IOBJECT_H
#define IOBJECT_H

#include <QObject>
#include <memory>

#include "Memory/memorywrapper.h"
#include "scriptwrapper.h"
#include "qqmlwrapper.h"
#include "commander.h"

class WindowManager;
class ObjectManager;

class IObject : public QObject
{
  Q_OBJECT
  Q_PROPERTY(MemoryWrapper* mem READ mem)
  Q_PROPERTY(WindowManager* WM READ getWM)
  //Q_PROPERTY(QObject* cm READ CM)
  Q_PROPERTY(QObject* OM READ getOM)
  //Q_PROPERTY(QString msg READ msg)
  Q_PROPERTY(QObject* qml READ getQml)

public:
  typedef std::shared_ptr<JSEngineWrapper> t_jswrapper;
  typedef std::shared_ptr<QmlEngineWrapper> t_qmlwrapper;

  explicit IObject(QObject *parent = 0);
  ~IObject();

  t_jswrapper scr() const;

  //QString msg() const;

  int countLinks() const;
  void incCountLinks();

  QObject *getOM();

  QmlEngineWrapper *getQml() const;

signals:
  // ������� ������ ������� � ������ receiver.
  // me - ������� � �������� ���������
  // path - ���� � �������� ������, ��� ��������� ������
  // params - ��� ��������� � ������� ��������� ������ (����� ���� ��������)
  void signalQuery(const QString &path, const QString &params);

  // ������� ������ ���������
  void sendLog(const QString &msg);

public slots:
  void release();

  MemoryWrapper *mem();
  // ��������� ������, ���������� � ������ �������, ����������� �� ���� path
  bool run(const QString &path, const QVariant &params=QVariant());

  // ��������� ������ � ��������� �������
  bool execute(const QString &text);

  // ������� � ������� ������.
  void reset();

  // ��������� QML ���
  bool runQML(const QString &text);

  // �������� ������ � ������.
  // name - ��� �������, �� �������� ����� ���������� � �������.
  // ���� name �� ������, �� � ������� ������ ���� ������ ��� objectName, ����� �� ���������.
  // ���� ������ � ����� ������ ����������, �� �� ���������.
  bool addObject(QObject *obj, const QString &name);
  // ������� ����� ��� ���������� ������������ ������
  QObject *getObject(const QString &name);

  //

  // �������� ���������
  void add_msg(const QString &msg);
  void log(const QString &msg);

  // ������� ��������
  // ���� ������ �� ������, �� �������. ������� ������
  WindowManager *getWM();

  //
  QObject *CM();

  // ������� ������� ������� � ������ receiver.
  // me - ������� � �������� ���������
  // path - ���� � �������� ������, ��� ��������� ������
  // params - ��� ��������� � ������� ��������� ������ (����� ���� ��������)
  void sendQuery(const QString &receiver, const QString &path, const QString &params);

protected slots:
  void onQuery(const QString &path, const QString &params);
	void onObjectNameChanged();

private:
  int countLinks_ = 0;
	MemoryWrapper *mem_ = nullptr;// ������ ������� � ���� ������
	t_jswrapper scr_;// ������ �� ����� JavaScript, ������������ ��� �����������
	QmlEngineWrapper *qml_ = nullptr;// ������ �� ������ � QML �������
  QString msg_;// ��������� �� ����������� ���������
	WindowManager *WM_ = nullptr;// ������� ��������
	ObjectManager *OM_ = nullptr;
	Commander *CM_ = nullptr;// �������� ������
};

Q_DECLARE_METATYPE(IObject*)

#endif // IOBJECT_H
