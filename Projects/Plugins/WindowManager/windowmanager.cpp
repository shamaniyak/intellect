#include "windowmanager.h"

#include <Src/GUI/menu.h>
#include <Src/GUI/toolbar.h>
#include "Src/GUI/filedialog.h"
#include "Src/GUI/graphicsview.h"
#include "Src/GUI/MainWindowEx/mainwindowex.h"

#include <QtWidgets>
#include <QProgressBar>

WindowManager::WindowManager(QObject *parent) : QObject(parent)
{

}

void WindowManager::makeParent(QWidget *&parent)
{
  if(!parent)
    parent = main_window_;
}

QObject *WindowManager::createWidget(QWidget *parent)
{
  //makeParent(parent);

  return new QWidget(parent);
}

QObject *WindowManager::createLabel(QWidget *parent)
{
  //makeParent(parent);
  return new QLabel(parent);
}

QObject *WindowManager::createLineEdit(QWidget *parent)
{
  //makeParent(parent);
  return new QLineEdit(parent);
}

QObject *WindowManager::createTextEdit(QWidget *parent)
{
  //makeParent(parent);
  return new QTextEdit(parent);
}

QObject *WindowManager::createPlainTextEdit(QWidget *parent)
{
  //makeParent(parent);
  return new QPlainTextEdit(parent);
}

QObject *WindowManager::createPushButton(QWidget *parent)
{
  //makeParent(parent);
  return new QPushButton(parent);
}

QObject *WindowManager::createListView(QWidget *parent)
{
  //makeParent(parent);
  return new QListView(parent);
}

QObject *WindowManager::createTreeView(QWidget *parent)
{
  //makeParent(parent);
  auto treeView = new QTreeView(parent);
  return treeView;
}

QObject *WindowManager::createTableView(QWidget *parent)
{
  //makeParent(parent);
  return new QTableView(parent);
}

QObject *WindowManager::createDialog(QWidget *parent)
{
  //makeParent(parent);
  return(new QDialog(parent));
}

QObject *WindowManager::createGraphicsView(QWidget *parent)
{
  //makeParent(parent);
  return(new GraphicsView(parent));
}

QObject *WindowManager::createToolBar(QWidget *parent)
{
  //makeParent(parent);
  return(new ToolBar(parent));
}

QObject *WindowManager::createFileDialog(QWidget *parent)
{
  //makeParent(parent);
  auto dlg = new FileDialog(parent);
  return dlg;
}

QObject *WindowManager::createMenu(QWidget *parent)
{
  //makeParent(parent);
  Menu *menu = new Menu(parent);
  return menu;
}

QWidget *WindowManager::createProgressBar(QWidget *parent)
{
  //makeParent(parent);
  return new QProgressBar(parent);
}

QMainWindow *WindowManager::main_window() const
{
  return main_window_;
}

void WindowManager::setMain_window(QMainWindow *main_window)
{
  main_window_ = main_window;
}

QWidget *WindowManager::createMainWindow(QWidget *parent)
{
  MainWindowEx *wgt = new MainWindowEx(parent);
  return wgt;
}

QObject *WindowManager::createLayout()
{
  return new QHBoxLayout();
}

int WindowManager::getVersion()
{
  return 1;
}
