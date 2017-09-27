#include "mainwindowex.h"
#include "ui_mainwindowex.h"
#include "gui/dialogmemoryeditor.h"
#include "logview.h"
#include "scripteditor.h"
#include "memorytreeview.h"
#include "intellect.h"
#include "menu.h"

MainWindowEx::MainWindowEx(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindowEx)
{
  ui->setupUi(this);
}

MainWindowEx::~MainWindowEx()
{
  delete ui;
}

QWidget *MainWindowEx::createNewDoc(QWidget *wgt, bool deleteOnClose)
{
  //QWidget *wgt = new QWidget;
  if(!wgt)
    return nullptr;

  QMdiSubWindow *sw = ui->mdiArea->addSubWindow(wgt);
  wgt->setAttribute(Qt::WA_DeleteOnClose, deleteOnClose);
  sw->setAttribute(Qt::WA_DeleteOnClose);
  sw->setWindowTitle("New");
  sw->showMaximized();

  auto list = ui->mdiArea->findChildren<QTabBar*>();
  for(auto &x: list)
    x->setExpanding(false);

  return sw;
}

QWidget *MainWindowEx::createNewDockWidget(QWidget *wgt, bool deleteOnClose)
{
  if(!wgt)
    return nullptr;
  QDockWidget *pdoc = new QDockWidget("New", this);
  pdoc->setWidget(wgt);
  pdoc->setAttribute(Qt::WA_DeleteOnClose, deleteOnClose);
  addDockWidget(Qt::LeftDockWidgetArea, pdoc);

  return pdoc;
}

void MainWindowEx::addToolBar(QObject *tb)
{
  auto ptb = qobject_cast<QToolBar *>(tb);
  if(ptb)
    QMainWindow::addToolBar(Qt::TopToolBarArea, ptb);
}

QObject *MainWindowEx::getToolBar(const QString &name)
{
  auto tb = this->findChild<QToolBar*>(name);
  return tb;
}

QWidget *MainWindowEx::getMenuBar()
{
  return menuBar();
}

QObject *MainWindowEx::addMenu(const QString &name)
{
  auto menu = new Menu();
  menu->setTitle(name);
  addMenu(menu);
  return menu;
}

void MainWindowEx::addMenu(QObject *menu)
{
  auto pmenu = qobject_cast<QMenu*>(menu);
  if(pmenu) {
    menuBar()->addMenu(pmenu);
  }
}

QObject *MainWindowEx::getMenu(const QString &name)
{
  auto menu = menuBar()->findChild<QMenu*>(name);
  return menu;
}

void MainWindowEx::closeEvent(QCloseEvent *event)
{
  QMainWindow::closeEvent(event);
}
