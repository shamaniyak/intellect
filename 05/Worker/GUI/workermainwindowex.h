#ifndef WORKERMAINWINDOWEX_H
#define WORKERMAINWINDOWEX_H

#include "mainwindowex.h"

#include <QtWidgets>

class Worker;
class MemoryTreeView;
class DialogMemoryEditor;
class ScriptEditor;

class WorkerMainWindowEx : public MainWindowEx
{
  Q_OBJECT

public:
  explicit WorkerMainWindowEx(Worker *i, QWidget *parent = 0);
  ~WorkerMainWindowEx();

public slots:
  void showMemoryView(bool show = true);
  void showEditor(bool show = true);
  void showLogView(bool show = true);

private slots:
  void on_action_triggered();
  void on_action_Editor_triggered();

protected:

  virtual void closeEvent(QCloseEvent *event);

  void createMemoryView();
  void createBaseEditor();
  void createLogView();
  void createScriptEditor();
  void createActions();
  void createToolBar();

  void initIntellect();

  void loadPlugins();

  bool canClose();

  void loadSettings();

  void saveSettings();

private:
  Worker *intellect_ = 0;
  MemoryTreeView *treeView_ = 0;
  DialogMemoryEditor *memEditor_ = 0;
  ScriptEditor *scriptEditor_ = 0;
  QWidget *treeDockWidget_ = 0;
  QWidget *editorDocWidget_ = 0;
  QWidget *logDockWidget_ = 0;
};

#endif // WORKERMAINWINDOWEX_H
