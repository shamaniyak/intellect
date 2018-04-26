#include "windowmanager.h"

#include <Src/GUI/menu.h>
#include <Src/GUI/toolbar.h>
#include "Src/GUI/filedialog.h"
#include "Src/GUI/MemoryView/memorytableview.h"
#include "Src/GUI/MemoryView/memorylistview.h"
#include "Src/GUI/MemoryView/memorytreeview.h"
#include "Src/GUI/memoryeditorwgt.h"
#include "Src/Memory/qmemorymodel.h"
#include "Src/GUI/graphicsview.h"
#include "Src/GUI/MainWindowEx/mainwindowex.h"
#include "Src/Memory/memorycompareproxymodel.h"
#include "Src/GUI/ScriptEditor/scripteditor.h"

#include <QtWidgets>
#include <QProgressBar>

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
  return new MemoryListView(parent);
}

QObject *WindowManager::createTreeView(MemoryWrapper *mem, QWidget *parent)
{
  //makeParent(parent);
  auto treeView = new QTreeView(parent);
  //auto model = new QMemoryModel(treeView);
  //model->setMem(mem);
  if(mem)
    treeView->setModel(mem);
  return treeView;
}

QObject *WindowManager::createTableView(QWidget *parent)
{
  //makeParent(parent);
  return new MemoryTableView(parent);
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

QWidget *WindowManager::createScriptEditor()
{
  return new ScriptEditor();
}

QObject *WindowManager::createMemoryView(QWidget *parent)
{
  //makeParent(parent);
  return new MemoryTreeView(parent);
}

QObject *WindowManager::createMemoryEditor(QWidget *parent)
{
  //makeParent(parent);
  return new MemoryEditorWgt(parent);
}

QObject *WindowManager::createMemoryCompare()
{
  return new MemoryCompareProxyModel();
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
