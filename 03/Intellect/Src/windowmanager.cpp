#include "windowmanager.h"
#include <QtWidgets>
#include <toolbar.h>
#include "filedialog.h"
#include "memorytableview.h"
#include "memorylistview.h"
#include "memorytreeview.h"
#include "memoryeditor.h"
#include "qmemorymodel.h"
#include "graphicsview.h"

WindowManager::WindowManager(QObject *parent) : QObject(parent),
  top_widget_(new QWidget())
{

}

void WindowManager::makeParent(QWidget *&parent)
{
  if(!parent)
    parent = top_widget_.get();
}

QObject *WindowManager::createWidget(QWidget *parent)
{
  makeParent(parent);

  return new QWidget(parent);
}

QObject *WindowManager::createLabel(QWidget *parent)
{
  makeParent(parent);
  return new QLabel(parent);
}

QObject *WindowManager::createLineEdit(QWidget *parent)
{
  makeParent(parent);
  return new QLineEdit(parent);
}

QObject *WindowManager::createTextEdit(QWidget *parent)
{
  makeParent(parent);
  return new QTextEdit(parent);
}

QObject *WindowManager::createPlainTextEdit(QWidget *parent)
{
  makeParent(parent);
  return new QPlainTextEdit(parent);
}

QObject *WindowManager::createPushButton(QWidget *parent)
{
  makeParent(parent);
  return new QPushButton(parent);
}

QObject *WindowManager::createListView(QWidget *parent)
{
  makeParent(parent);
  return new MemoryListView(parent);
}

QObject *WindowManager::createTreeView(MemoryWrapper *mem, QWidget *parent)
{
  makeParent(parent);
  auto treeView = new QTreeView(parent);
  auto model = new QMemoryModel(treeView);
  model->setMem(mem);
  treeView->setModel(model);
  return treeView;
}

QObject *WindowManager::createTableView(QWidget *parent)
{
  makeParent(parent);
  return new MemoryTableView(parent);
}

QObject *WindowManager::createDialog(QWidget *parent)
{
  makeParent(parent);
  return(new QDialog(parent));
}

QObject *WindowManager::createGraphicsView(QWidget *parent)
{
  makeParent(parent);
  return(new GraphicsView(parent));
}

QObject *WindowManager::createToolBar(QWidget *parent)
{
  makeParent(parent);
  return(new ToolBar(parent));
}

QObject *WindowManager::createFileDialog(QWidget *parent)
{
  makeParent(parent);
  auto *dlg = new FileDialog(parent);
  return dlg;
}

QObject *WindowManager::createMemoryView(QWidget *parent)
{
  makeParent(parent);
  return new MemoryTreeView(parent);
}

QObject *WindowManager::createMemoryEditor(QWidget *parent)
{
  makeParent(parent);
  return new MemoryEditor(parent);
}

QMainWindow *WindowManager::main_window() const
{
  return main_window_;
}

void WindowManager::setMain_window(QMainWindow *main_window)
{
  main_window_ = main_window;
}
