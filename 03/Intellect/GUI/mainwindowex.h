#ifndef MAINWINDOWEX_H
#define MAINWINDOWEX_H

#include <QMainWindow>
#include <QtWidgets>

namespace Ui {
class MainWindowEx;
}

class Intellect;
class MemoryTreeView;
class DialogMemoryEditor;
class ScriptEditor;

class MainWindowEx : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindowEx(Intellect *i, QWidget *parent = 0);
  ~MainWindowEx();

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

#endif // MAINWINDOWEX_H
