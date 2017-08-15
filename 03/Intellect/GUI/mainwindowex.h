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
  QWidget *createNewDoc(QWidget *wgt);
  QWidget *createNewDockWidget(QWidget *wgt);

  void addToolBar(QObject *tb);
  QObject *getToolBar(const QString &name);

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
};

#endif // MAINWINDOWEX_H
