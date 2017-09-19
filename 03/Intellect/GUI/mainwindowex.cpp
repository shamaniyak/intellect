#include "mainwindowex.h"
#include "ui_mainwindowex.h"
#include "gui/dialogmemoryeditor.h"
#include "logview.h"
#include "scripteditor.h"
#include "memorytreeview.h"
#include <intellect.h>
//#include "windowmanager.h"

MainWindowEx::MainWindowEx(Intellect *i, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindowEx),
  intellect_(i)
{
  ui->setupUi(this);

  //setAttribute(Qt::WA_AlwaysShowToolTips);
  // создать редактор базы знаний
  //createBaseEditor();

  createMemoryView();
  createLogView();
  createScriptEditor();
  //
  initIntellect();
  //
  //loadPlugins();
  //
  loadSettings();
  //
  //createActions();
  //createToolBar();
}

MainWindowEx::~MainWindowEx()
{
  intellect_->getOM()->Del("TreeMemory");
  intellect_->getOM()->Del("TreeMenu");

  saveSettings();

  delete ui;
}

void MainWindowEx::initIntellect()
{
  if(!intellect_)
    return;

  intellect_->setMainWindow(this);
  // Добавить скриптовые объекты
  intellect_->addObject(treeView_, "TreeMemory");
  intellect_->addObject(treeView_->pmenu(), "TreeMenu");
  intellect_->addObject(scriptEditor_, "Editor");
}

void MainWindowEx::loadPlugins()
{
  if (intellect_->loadPlugins() == 0) {
    QMessageBox::critical(0, "", "No plugins.");
    return;
  }
}

bool MainWindowEx::canClose()
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

void MainWindowEx::createMemoryView()
{
  // Дерево памяти
  treeView_ = new MemoryTreeView();
  treeView_->setMem(intellect_->obj()->mem());
  // новая вкладка для дерева
  auto wgt = createNewDockWidget(treeView_);
  wgt->setWindowTitle(tr("Память"));
}

void MainWindowEx::createBaseEditor()
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

void MainWindowEx::createLogView()
{
  auto log = new LogView();
  log->setAlg(intellect_);
  auto wgt = static_cast<QDockWidget*>( createNewDockWidget(log) );
  wgt->setWindowTitle(tr("Лог"));
  addDockWidget(Qt::BottomDockWidgetArea, wgt);
}

void MainWindowEx::createScriptEditor()
{
  scriptEditor_ = new ScriptEditor();
  scriptEditor_->setIntellect(intellect_);
  scriptEditor_->setMem(intellect_->obj()->mem());
  auto wgt = createNewDoc(scriptEditor_);
  wgt->setWindowTitle(tr("Редактор"));
}

void MainWindowEx::createActions()
{

}

void MainWindowEx::createToolBar()
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


QWidget *MainWindowEx::createNewDoc(QWidget *wgt)
{
  //QWidget *wgt = new QWidget;
  if(!wgt)
    return nullptr;

  QMdiSubWindow *sw = ui->mdiArea->addSubWindow(wgt);
  wgt->setAttribute(Qt::WA_DeleteOnClose);
  wgt->setWindowTitle("New");
  wgt->showMaximized();

  auto list = ui->mdiArea->findChildren<QTabBar*>();
  for(auto &x: list)
    x->setExpanding(false);

  return sw;
}

QWidget *MainWindowEx::createNewDockWidget(QWidget *wgt)
{
  if(!wgt)
    return nullptr;
  QDockWidget *pdoc = new QDockWidget("New", this);
  pdoc->setAttribute(Qt::WA_DeleteOnClose);
  pdoc->setWidget(wgt);
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

void MainWindowEx::on_action_triggered()
{
  //todo: нужно оставновить все скрипты

  close();
}

void MainWindowEx::closeEvent(QCloseEvent *event)
{
  if(canClose())
  {
    event->accept();

    intellect_->stop();
  }
  else
    event->ignore();
  //QMainWindow::closeEvent(event);
}

void MainWindowEx::loadSettings()
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

void MainWindowEx::saveSettings()
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
