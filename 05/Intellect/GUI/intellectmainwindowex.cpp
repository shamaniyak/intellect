#include "intellectmainwindowex.h"
#include "ui_mainwindowex.h"
#include "Src/GUI/logview.h"
#include "Src/GUI/ScriptEditor/scripteditor.h"
#include "Src/GUI/MemoryView/memorytreeview.h"
#include "Src/intellect.h"
#include "Src/GUI/menu.h"

IntellectMainWindowEx::IntellectMainWindowEx(Intellect *i, QWidget *parent) :
  MainWindowEx(parent),
  intellect_(i)
{
  //setAttribute(Qt::WA_AlwaysShowToolTips);
  // создать редактор базы знаний
  //createBaseEditor();

  createMemoryView();
  createLogView();
  createScriptEditor();
  //
  initIntellect();
  //
  loadSettings();
  //
  createActions();
  //createToolBar();
}

IntellectMainWindowEx::~IntellectMainWindowEx()
{
  intellect_->getOM()->Del("TreeMemory");
  intellect_->getOM()->Del("TreeMenu");

  saveSettings();
}

void IntellectMainWindowEx::initIntellect()
{
  if(!intellect_)
    return;

  intellect_->setMainWindow(this);
  // Добавить скриптовые объекты
  intellect_->addObject(treeView_, "TreeMemory");
  intellect_->addObject(treeView_->pmenu(), "TreeMenu");
  //intellect_->addObject(scriptEditor_, "Editor");
  auto oTreeMenu = qobject_cast<IObject*>(intellect_->getOM()->Get("TreeMenu"));
  if(oTreeMenu)
    oTreeMenu->addObject(treeView_, "TreeMemory");
}

void IntellectMainWindowEx::loadPlugins()
{
  if (intellect_->loadPlugins() == 0) {
    QMessageBox::critical(0, "", "No plugins.");
    return;
  }
}

bool IntellectMainWindowEx::canClose()
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

void IntellectMainWindowEx::createBaseEditor()
{

}

void IntellectMainWindowEx::createMemoryView()
{
  // Дерево памяти
  treeView_ = new MemoryTreeView();
  treeView_->setMem(intellect_->obj()->mem());
  // новая вкладка для дерева
  treeDockWidget_ = createNewDockWidget(treeView_, false);
  treeDockWidget_->setWindowTitle(tr("Память"));
}

void IntellectMainWindowEx::createLogView()
{
  auto log = new LogView();
  connect(intellect_, &TAlgorithm::addResult, log, &LogView::addResult);

  logDockWidget_ = createNewDockWidget(log, false);
  auto wgt = static_cast<QDockWidget*>(logDockWidget_);
  wgt->setWindowTitle(tr("Лог"));
  addDockWidget(Qt::BottomDockWidgetArea, wgt);
}

void IntellectMainWindowEx::createScriptEditor()
{
  scriptEditor_ = new ScriptEditor(this);
  auto iobj = intellect_->getObject("Editor");
  scriptEditor_->setIobj(iobj);
  scriptEditor_->setMem(intellect_->obj()->mem());

  editorDocWidget_ = createNewDoc(scriptEditor_, false);
  editorDocWidget_->setWindowTitle(tr("Редактор"));
}

void IntellectMainWindowEx::createActions()
{
//  Menu* menu = new Menu();
//  menu->setTitle("NewMenu");
//  addMenu(menu);
//  if(menu)
//    menu->createAction("newAction");
}

void IntellectMainWindowEx::createToolBar()
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

void IntellectMainWindowEx::showMemoryView(bool show)
{
  treeDockWidget_->setVisible(show);
}

void IntellectMainWindowEx::showEditor(bool show)
{
  if(!show)
    editorDocWidget_->close();
  else
    createScriptEditor();
}

void IntellectMainWindowEx::showLogView(bool show)
{
  logDockWidget_->setVisible(show);
}

void IntellectMainWindowEx::on_action_triggered()
{
  //todo: нужно оставновить все скрипты

  close();
}

void IntellectMainWindowEx::on_action_Editor_triggered()
{
  showMemoryView(true);
  showEditor(true);
  showLogView(true);
}

void IntellectMainWindowEx::closeEvent(QCloseEvent *event)
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

void IntellectMainWindowEx::loadSettings()
{
  QString filePath = QApplication::applicationDirPath() + "/settings.moi";
  MemoryWrapper mSettings;

  if(mSettings.open(filePath))
  {
    auto meSettings = mSettings.get("Intellect\\Settings");

    if(meSettings)
    {
      auto meTree = meSettings->get("treeView");
      if(meTree)
      {
        int w = meTree->get("Width")->val().toInt();
        int h = meTree->get("Height")->val().toInt();

        // развернутые элементы
        auto me = meTree->get("Expanded");
        treeView_->loadExpandItems(me);

        treeView_->resize(w, h);

        // текущий элемент
        if(intellect_)
        {
          QString selectedPath;
          me = meTree->get("Selected");
          if(me) selectedPath = me->val().toString();
          auto selected = intellect_->obj()->mem()->get(selectedPath);
          intellect_->obj()->mem()->setSelected(selected);
        }
      }

      auto meMaximized = meSettings->get("maximized");
      if(meMaximized) {
        if(meMaximized->val().toBool())
          this->showMaximized();
        else
          this->showNormal();
      }
    }
  }

}

void IntellectMainWindowEx::saveSettings()
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
    // размеры дерева
    auto meTree = meSettings->add("treeView");
    meTree->add("Width")->setVal(treeView_->size().width());
    meTree->add("Height")->setVal(treeView_->size().height());
    // развернутые элементы
    auto me = meTree->add("Expanded");
    treeView_->saveExpandItems(me);
    // текущий элемент
    QString selectedPath;
    auto selected = intellect_->obj()->mem()->getSelected();
    if(selected) selectedPath = selected->getPath();
    meTree->add("Selected")->setVal(selectedPath);
    // на весь экран
    auto meMaximized = meSettings->add("maximized");
    meMaximized->setVal(this->isMaximized());
  }
}
