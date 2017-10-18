#include "mainwindowex.h"
#include "ui_mainwindowex.h"
#include "Src/GUI/menu.h"
#include <Src/GUI/statusbar.h>

MainWindowEx::MainWindowEx(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindowEx)
{
  ui->setupUi(this);

  setStatusBar(new StatusBar());
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

QWidget *MainWindowEx::createDocument(QWidget *wgt)
{
  if(!wgt)
    return nullptr;
  return createNewDoc(wgt);
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

QWidget *MainWindowEx::createDockWidget(QWidget *wgt, int area, int orientation)
{
  if(!wgt)
    return nullptr;
  QDockWidget *pdoc = new QDockWidget("New", this);
  pdoc->setWidget(wgt);
  //pdoc->setAttribute(Qt::WA_DeleteOnClose, true);
  auto a = (Qt::DockWidgetArea)area;
  auto o = (Qt::Orientation)orientation;
  addDockWidget(a, pdoc, o);

  return pdoc;
}

void MainWindowEx::tabifyDockWidget(QDockWidget *first, QDockWidget *second)
{
  if(!first || !second) return;

  QMainWindow::tabifyDockWidget(first, second);
}

void MainWindowEx::addToolBar(QObject *tb)
{
  auto ptb = qobject_cast<QToolBar *>(tb);
  if(ptb)
    QMainWindow::addToolBar(Qt::TopToolBarArea, ptb);
}

QWidget *MainWindowEx::getToolBar(const QString &name)
{
  auto tb = this->findChild<QToolBar*>(name);
  return tb;
}

QWidget *MainWindowEx::getMenuBar()
{
  return menuBar();
}

QWidget *MainWindowEx::getStatusBar()
{
  return QMainWindow::statusBar();
}

QObject *MainWindowEx::createMenu(const QString &title)
{
  auto menu = new Menu();
  menu->setTitle(title);
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
  //qDebug() << name;
  auto menu = menuBar()->findChild<QMenu*>(name);
  for(auto act: menuBar()->actions())
  {
    if(act->menu() && act->menu()->objectName() == name)
      menu = act->menu();
  }
  return menu;
}

void MainWindowEx::deleteMenu(QObject *menu)
{
  delete menu;
}

void MainWindowEx::closeEvent(QCloseEvent *event)
{
  QMainWindow::closeEvent(event);
}
