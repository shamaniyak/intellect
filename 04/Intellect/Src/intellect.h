/****************************************************************************
**
** Copyright (C) 2015-2017 Aleksandr Abramov
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

#ifndef INTELLECT_H
#define INTELLECT_H

#include <memory>
#include <Src/iobject.h>
#include <Src/talgorithm.h>

class WindowManager;
class QMainWindow;

class Intellect : public TAlgorithm
{
  Q_OBJECT
  Q_PROPERTY(QObject* parent READ parent)
  Q_PROPERTY(IObject* obj READ obj)
  Q_PROPERTY(QObject* mainWindow READ getMainWindowQObject)

public:
  explicit Intellect(QObject *parent = 0);
  ~Intellect();

  IObject* obj() const;

  QMainWindow *getMainWindow() const;
  void setMainWindow(QMainWindow *mw);

  QString compileText(const QString &str);

  Q_INVOKABLE int loadPlugins();

  void event(QObject *obj, QEvent *ev);

public slots:
  void stop();

  void recreateMemory();

protected slots:
  void on_addObject(QObject *obj);
  void on_start();

protected:
  bool keyEvent(QObject *obj, QEvent *ev);

  QObject *getMainWindowQObject() const;

private:
  IObject *obj_ = 0;
  QMainWindow *mainWindow_ = 0;
};

#endif // INTELLECT_H
