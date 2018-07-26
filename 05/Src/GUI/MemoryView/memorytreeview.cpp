#include "memorytreeview.h"

#include "Src/Memory/qmemorymodel.h"
#include "Src/Memory/qmemoryselectionmodel.h"
#include "Src/Memory/memorycompareproxymodel.h"
#include "memoryeditordelegate.h"
#include "Src/GUI/menu.h"

#include <QtDebug>
#include <QMenu>
#include <QMouseEvent>
#include <QHeaderView>
#include <QPushButton>
#include <QLineEdit>

MemoryTreeView::MemoryTreeView(QWidget *parent):
  QTreeView(parent)
{
//  qDebug() << "MemoryTreeView::MemoryTreeView()";
  createModels();

  //auto delegate = new MemoryEditorDelegate;
  //setItemDelegate(delegate);

  createContextMenu();

  setup();

  connectSlots();
}

MemoryTreeView::~MemoryTreeView()
{
//  qDebug() << "MemoryTreeView::~MemoryTreeView()";
  disconnectSlots();
}

void MemoryTreeView::createModels()
{
  selectionModel_ = new QMemorySelectionModel();
  selectionModel_->setObjectName("selectionModel");
  //setSelectionModel(selectionModel_);
}

void MemoryTreeView::createContextMenu()
{
  m_pmenu = new Menu(this);
  m_pmenu->setObjectName("menu");
  m_menu = m_pmenu;
}

void MemoryTreeView::connectSlots()
{
  connect(this, &QTreeView::expanded, this, &MemoryTreeView::on_itemExpanded);
  connect(this, &QTreeView::collapsed, this, &MemoryTreeView::on_itemCollapsed);
  connect(selectionModel_, &QMemorySelectionModel::currentRowChanged, this, &MemoryTreeView::on_currentRowChanged);
}

void MemoryTreeView::disconnectSlots()
{
  this->disconnect(this);
  if(selectionModel_)
    selectionModel_->disconnect(this);
  if(mem_)
    mem_->disconnect(this);
}

void MemoryTreeView::initDragDrop()
{
  setSelectionMode(QAbstractItemView::ExtendedSelection);
  this->setDragEnabled(true);
  this->setAcceptDrops(true);
  this->setDropIndicatorShown(true);
  setDragDropMode(QAbstractItemView::InternalMove);
}

void MemoryTreeView::setup()
{
  initDragDrop();
//  this->setSelectionMode(QAbstractItemView::ExtendedSelection);
//  this->setDragEnabled(true);
//  this->setAcceptDrops(true);
//  this->setDropIndicatorShown(true);
}

void MemoryTreeView::contextMenuEvent(QContextMenuEvent *pe)
{
  emit contextMenuPopup();

  m_pmenu->exec(pe->globalPos());
}

void MemoryTreeView::on_itemCollapsed(const QModelIndex &item)
{
  auto me = mem_->getMeByIndex(item);
  if(!me) return;
  QString path = me.getPath();

  expanded_.removeOne(path);
}

void MemoryTreeView::on_itemExpanded(const QModelIndex &item)
{
  auto me = mem_->getMeByIndex(item);
  if(!me) return;
  QString path = me.getPath();

  if(!expanded_.contains(path))
    expanded_ << path;
}

QWidget *MemoryTreeView::createWidget(const QModelIndex &index)
{
  auto wgt = new QLineEdit;
  auto text = index.model()->data(index).toString();
  wgt->setText(text);
  return wgt;
}

void MemoryTreeView::on_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
  //setIndexWidget(previous, nullptr);
  //setIndexWidget(current, createWidget(current));
}

MemoryWrapper *MemoryTreeView::getMem() const
{
  return mem_;
}

void MemoryTreeView::setMem(MemoryWrapper *mem)
{
  if(mem && mem != mem_)
  {
    mem_ = mem;
    if(mem_)
    {
      setModel(mem_);

      if(selectionModel_)
      {
        selectionModel_->setMem(mem_);
        setSelectionModel(selectionModel_);
      }

      this->header()->resizeSection(0, 200);

      this->setCurrentIndex(mem_->getIndexByMe(mem_->getSelected()));
    }

    expandItems();
  }
}

void MemoryTreeView::expandItems()
{
  if(!mem_)
    return;

  for(auto path: expanded_)
  {
    MEWrapper me = mem_->get(path);
    if(me)
    {
      QModelIndex mi = mem_->getIndexByMe(me);
      this->setExpanded(mi, true);
    }
  }
}

void MemoryTreeView::loadExpandItems(MEWrapper &me)
{
  if(!me)
    return;
  expanded_.clear();
  expanded_ = me.val().toString().split("\n");

  expandItems();
}

void MemoryTreeView::saveExpandItems(MEWrapper &me)
{
  if(!me)
    return;
  QVariant val = expanded_.join("\n");
  me.setVal(val);
}

MemoryWrapper *MemoryTreeView::memHeader()
{
  if(!header_) {
    header_ = new QMemoryModel(this);

  }
  return header_;
}

QObject *MemoryTreeView::menu() const
{
  return m_menu;
}

void MemoryTreeView::compareWith(MemoryWrapper *srcMem)
{
  if(!srcMem) return;
  if(!mem_) return;
  if(!memoryCompare_)
    memoryCompare_ = new MemoryCompareProxyModel(this);
  memoryCompare_->setSourceMemory(static_cast<QMemoryModel*>(mem_));
  memoryCompare_->setCompareMemory(static_cast<QMemoryModel*>(srcMem));
  if(!memoryCompare_->compare())
      return;

  if(selectionModel_)
  {
    //selectionModel->setMem(mem_);
    selectionModel_->setModel(nullptr);
  }
  setModel(memoryCompare_);
}

void MemoryTreeView::deleteCompare()
{
  delete memoryCompare_;
  memoryCompare_ = 0;
  setModel(mem_);
}

void MemoryTreeView::setMenu(QObject *menu)
{
  if (m_menu == menu)
    return;

  if(m_menu)
    m_menu->deleteLater();


  m_menu = qobject_cast<QMenu*>(menu);
  emit menuChanged(m_menu);
}

QMenu *MemoryTreeView::pmenu() const
{
  return m_pmenu;
}

void MemoryTreeView::mousePressEvent ( QMouseEvent *event )
{
  if( event->button() == Qt::LeftButton)
  {

  }
  else if( event->button() == Qt::RightButton)
  {

  }

  QTreeView::mousePressEvent(event);
}


