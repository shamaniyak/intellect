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

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>
#include <memory>

class QWidget;
class QMainWindow;
class MemoryWrapper;

class WindowManager : public QObject
{
  Q_OBJECT
  //Q_PROPERTY(QMainWindow* mainWindow READ main_window)

public:

  explicit WindowManager(QObject *parent = 0);

  QMainWindow *main_window() const;
  void setMain_window(QMainWindow *main_window);

signals:

public slots:
  QWidget * createMainWindow(QWidget *parent = 0);
  QObject * createLayout();
  QObject * createWidget(QWidget *parent = 0);
  QObject * createLabel(QWidget *parent = 0);
  QObject * createLineEdit(QWidget *parent = 0);
  QObject * createTextEdit(QWidget *parent = 0);
  QObject * createPlainTextEdit(QWidget *parent = 0);
  QObject * createPushButton(QWidget *parent = 0);
  QObject * createListView(QWidget *parent = 0);
  QObject * createTreeView(MemoryWrapper *mem, QWidget *parent = 0);
  QObject * createTableView(QWidget *parent = 0);
  QObject *createDialog(QWidget *parent = 0);
  QObject *createGraphicsView(QWidget *parent = 0);
  QObject *createToolBar(QWidget *parent = 0);
  QObject *createFileDialog(QWidget *parent = 0);
  QObject *createMenu(QWidget *parent = 0);
  QWidget *createProgressBar(QWidget *parent = 0);
  QWidget *createScriptEditor();

  QObject * createMemoryView(QWidget *parent = 0);
  QObject * createMemoryEditor(QWidget *parent = 0);
  QObject * createMemoryCompare();

protected:
  void makeParent(QWidget *&parent);

private:
  std::shared_ptr<QWidget> top_widget_;
  QMainWindow *main_window_ =0;
};

#endif // WINDOWMANAGER_H
