#include "workermainwindowex.h"
#include "ui_mainwindowex.h"
#include "gui/dialogmemoryeditor.h"
#include "logview.h"
#include "scripteditor.h"
#include "memorytreeview.h"
#include "Src/worker.h"
#include "menu.h"

WorkerMainWindowEx::WorkerMainWindowEx(Worker *i, QWidget *parent) :
  MainWindowEx(parent),
  intellect_(i)
{
  //setAttribute(Qt::WA_AlwaysShowToolTips);

  //createMemoryView();
  createLogView();
  //createScriptEditor();
  //
  initIntellect();
  //
  //loadSettings();
  //
  //createActions();
  //createToolBar();
}

WorkerMainWindowEx::~WorkerMainWindowEx()
{
  //intellect_->getOM()->Del("TreeMemory");
  //intellect_->getOM()->Del("TreeMenu");

  //saveSettings();
}

void WorkerMainWindowEx::initIntellect()
{
  if(!intellect_)
    return;

  intellect_->setMainWindow(this);
  // Добавить скриптовые объекты
  //intellect_->addObject(treeView_, "TreeMemory");
  //intellect_->addObject(treeView_->pmenu(), "TreeMenu");
  //intellect_->addObject(scriptEditor_, "Editor");
  //auto oTreeMenu = qobject_cast<IObject*>(intellect_->getOM()->Get("TreeMenu"));
  //if(oTreeMenu)
  //  oTreeMenu->addObject(treeView_, "TreeMemory");
}

void WorkerMainWindowEx::loadPlugins()
{
  if (intellect_->loadPlugins() == 0) {
    QMessageBox::critical(0, "", "No plugins.");
    return;
  }
}

bool WorkerMainWindowEx::canClose()
{
  if(intellect_->obj()->mem()->changed())
  {
    auto btn  = QMessageBox::question(0, "", "Memory changed. Save?",
                                      QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    if(btn == QMessageBox::Yes)
    {
      intellect_->obj()->mem()->save();
      return true;
    }
    if(btn == QMessageBox::No)
    {
      return true;
    }

    return false;
  }

  return true;
}

void WorkerMainWindowEx::createBaseEditor()
{
  if(!intellect_)
    return;

  memEditor_ = new DialogMemoryEditor();
  memEditor_->setAttribute(Qt::WA_DeleteOnClose);
  memEditor_->setIntellect(intellect_);
  // новая вкладка для редактора БЗ
  //auto wgt = createNewDoc(memEditor_);
  //wgt->setWindowTitle(tr("База знаний"));
}

void WorkerMainWindowEx::createMemoryView()
{
  // Дерево памяти
  treeView_ = new MemoryTreeView();
  treeView_->setMem(intellect_->obj()->mem());
  // новая вкладка для дерева
  treeDockWidget_ = createNewDockWidget(treeView_, false);
  treeDockWidget_->setWindowTitle(tr("Память"));
}

void WorkerMainWindowEx::createLogView()
{
  auto log = new LogView();
  log->setAlg(intellect_);
  logDockWidget_ = createNewDockWidget(log, false);
  auto wgt = static_cast<QDockWidget*>(logDockWidget_);
  wgt->setWindowTitle(tr("Лог"));
  addDockWidget(Qt::BottomDockWidgetArea, wgt);
}

void WorkerMainWindowEx::createScriptEditor()
{
  scriptEditor_ = new ScriptEditor(this);
  auto iobj = intellect_->getObject("Editor");
  scriptEditor_->setIobj(iobj);
  scriptEditor_->setMem(intellect_->obj()->mem());

  editorDocWidget_ = createNewDoc(scriptEditor_, false);
  editorDocWidget_->setWindowTitle(tr("Редактор"));
}

void WorkerMainWindowEx::createActions()
{
//  Menu* menu = new Menu();
//  menu->setTitle("NewMenu");
//  addMenu(menu);
//  if(menu)
//    menu->createAction("newAction");
}

void WorkerMainWindowEx::createToolBar()
{
  QToolBar *ptb = new QToolBar("Main");
  ptb->setObjectName("MainToolBar");

  auto act = ptb->addAction("+");
  act->setToolTip("Добавить");
  //connect(act, &QAction::triggered, )

  act = ptb->addAction("-");
  act->setToolTip("Удалить");

  addToolBar(ptb);
}

void WorkerMainWindowEx::showMemoryView(bool show)
{
  treeDockWidget_->setVisible(show);
}

void WorkerMainWindowEx::showEditor(bool show)
{
  if(!show)
    editorDocWidget_->close();
  else
    createScriptEditor();
}

void WorkerMainWindowEx::showLogView(bool show)
{
  logDockWidget_->setVisible(show);
}

void WorkerMainWindowEx::on_action_triggered()
{
  //todo: нужно оставновить все скрипты

  close();
}

void WorkerMainWindowEx::on_action_Editor_triggered()
{
  showMemoryView(true);
  showEditor(true);
  showLogView(true);
}

void WorkerMainWindowEx::closeEvent(QCloseEvent *event)
{
  if(canClose())
  {
    event->accept();

    intellect_->stop();
  }
  else
    event->ignore();
  //MainWindowEx::closeEvent(event);
}

void WorkerMainWindowEx::loadSettings()
{
  QString filePath = QApplication::applicationDirPath() + "/settings.moi";
  MemoryWrapper mSettings;

  if(mSettings.open(filePath))
  {
    auto meSettings = mSettings.get("Intellect\\Settings");

    if(meSettings)
    {
      if(memEditor_)
        memEditor_->LoadSettings(meSettings);

      auto meTree = meSettings->get("treeView");
      if(meTree)
      {
        int w = meTree->get("Width")->val().toInt();
        int h = meTree->get("Height")->val().toInt();

        // развернутые элементы
        auto me = meTree->get("Expanded");
        treeView_->loadExpandItems(me);

        treeView_->resize(w, h);
      }

    }
  }

}

void WorkerMainWindowEx::saveSettings()
{
  if(!intellect_)
    return;

  QString filePath = QApplication::applicationDirPath() + "/settings.moi";
  MemoryWrapper mSettings;
  mSettings.setAutosave(true);
  mSettings.setFilePath(filePath);

  auto meSettings = mSettings.add(nullptr, "Intellect")->add("Settings");

  if(meSettings)
  {
    if(memEditor_)
      memEditor_->SaveSettings(meSettings);

    // размеры дерева
    auto meTree = meSettings->add("treeView");
    meTree->add("Width")->setVal(treeView_->size().width());
    meTree->add("Height")->setVal(treeView_->size().height());
    // развернутые элементы
    auto me = meTree->add("Expanded");
    treeView_->saveExpandItems(me);
  }
}
