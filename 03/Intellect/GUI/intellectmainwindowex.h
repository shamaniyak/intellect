#ifndef INTELLECTMAINWINDOWEX_H
#define INTELLECTMAINWINDOWEX_H

#include "mainwindowex.h"

#include <QtWidgets>

class Intellect;
class MemoryTreeView;
class DialogMemoryEditor;
class ScriptEditor;

class IntellectMainWindowEx : public MainWindowEx
{
  Q_OBJECT

public:
  explicit IntellectMainWindowEx(Intellect *i, QWidget *parent = 0);
  ~IntellectMainWindowEx();

public slots:
  QWidget *createNewDoc(QWidget *wgt, bool deleteOnClose = true);
  QWidget *createNewDockWidget(QWidget *wgt, bool deleteOnClose = true);

  void addToolBar(QObject *tb);
  QObject *getToolBar(const QString &name);

  QWidget *getMenuBar();

  QObject *addMenu(const QString &name);
  void addMenu(QObject *menu);
  QObject *getMenu(const QString &name);

  void showMemoryView(bool show = true);
  void showEditor(bool show = true);
  void showLogView(bool show = true);

private slots:
  void on_action_triggered();

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
  Ui::MainWindowEx *ui;
  Intellect *intellect_ = 0;
  MemoryTreeView *treeView_ = 0;
  DialogMemoryEditor *memEditor_ = 0;
  ScriptEditor *scriptEditor_ = 0;
  QWidget *treeDockWidget_ = 0;
  QWidget *editorDocWidget_ = 0;
  QWidget *logDockWidget_ = 0;
};

#endif // INTELLECTMAINWINDOWEX_H
