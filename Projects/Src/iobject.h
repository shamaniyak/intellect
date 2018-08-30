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
  // Послать запрос объекту с именем receiver.
  // me - входные и выходные параметры
  // path - путь к элементу памяти, где находится скрипт
  // params - это параметры в формате командной страки (может быть скриптом)
  void signalQuery(const QString &path, const QString &params);

  // Послать сигнал сообщения
  void sendLog(const QString &msg);

public slots:
  void release();

  MemoryWrapper *mem();
  // Выполнить скрипт, записанный в памяти объекта, находящийся по пути path
  bool run(const QString &path, const QVariant &params=QVariant());

  // Выполнить скрипт в контексте объекта
  bool execute(const QString &text);

  // Очищает и удаляет скрипт.
  void reset();

  // Выполнить QML код
  bool runQML(const QString &text);

  // Добавить объект в скрипт.
  // name - имя объекта, по которому можно обращаться в скрипте.
  // Если name не задано, то у объекта должно быть задано имя objectName, иначе не добавится.
  // Если объект с таким именем существует, то не добавится.
  bool addObject(QObject *obj, const QString &name);
  // Создает новый или возвращает существующий объект
  QObject *getObject(const QString &name);

  //

  // Добавить сообщение
  void add_msg(const QString &msg);
  void log(const QString &msg);

  // Оконный мэнеджер
  // Если объект не создан, то создать. Вернуть объект
  WindowManager *getWM();

  //
  QObject *CM();

  // Послать команду объекту с именем receiver.
  // me - входные и выходные параметры
  // path - путь к элементу памяти, где находится скрипт
  // params - это параметры в формате командной страки (может быть скриптом)
  void sendQuery(const QString &receiver, const QString &path, const QString &params);

protected slots:
  void onQuery(const QString &path, const QString &params);
	void onObjectNameChanged();

private:
  int countLinks_ = 0;
	MemoryWrapper *mem_ = nullptr;// Данные объекта в виде дерева
	t_jswrapper scr_;// Скрипт на языке JavaScript, расширяющего его возможности
	QmlEngineWrapper *qml_ = nullptr;// Объект по работе с QML движком
  QString msg_;// Сообщение об выполненных операциях
	WindowManager *WM_ = nullptr;// Оконный мэнеджер
	ObjectManager *OM_ = nullptr;
	Commander *CM_ = nullptr;// Мэнеджер команд
};

Q_DECLARE_METATYPE(IObject*)

#endif // IOBJECT_H
