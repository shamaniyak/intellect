#ifndef INTELLECTMAINWINDOWEX_H
#define INTELLECTMAINWINDOWEX_H

#include "Src/GUI/MainWindowEx/mainwindowex.h"

#include <QtWidgets>

class Intellect;
class MemoryTreeView;
class ScriptEditor;

class IntellectMainWindowEx : public MainWindowEx
{
  Q_OBJECT

public:
  explicit IntellectMainWindowEx(Intellect *i, QWidget *parent = 0);
  ~IntellectMainWindowEx();

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
  Intellect *intellect_ = 0;
  MemoryTreeView *treeView_ = 0;
  ScriptEditor *scriptEditor_ = 0;
  QWidget *treeDockWidget_ = 0;
  QWidget *editorDocWidget_ = 0;
  QWidget *logDockWidget_ = 0;
};

#endif // INTELLECTMAINWINDOWEX_H
