#include "contextmenu.h"
#include "memorytreeview.h"
#include "mytreewidgetitem.h"
#include "qmemorymodel.h"
#include "qmemoryselectionmodel.h"

#include <QMenu>
#include <QMouseEvent>
#include <QHeaderView>

MemoryTreeView::MemoryTreeView(QWidget *parent):
  QTreeView(parent)
{
  initModels();

  createContextMenu();

  //initDragDrop();

  setup();

  connectSlots();
}

MemoryTreeView::~MemoryTreeView()
{
  disconnectSlots();
}

void MemoryTreeView::initModels()
{
  if(!model_)
  {
    model_ = new QMemoryModel(this);
    model_->setObjectName("model");
  }

  auto selectionModel = new QMemorySelectionModel(model_, this);
  selectionModel->setObjectName("selectionModel");
}

void MemoryTreeView::createContextMenu()
{
  m_pmenu = new ContextMenu(this);
  m_pmenu->setObjectName("menu");
}

void MemoryTreeView::connectSlots()
{
  connect(this, &QTreeView::expanded, this, &MemoryTreeView::on_itemExpanded);
  connect(this, &QTreeView::collapsed, this, &MemoryTreeView::on_itemCollapsed);
}

void MemoryTreeView::disconnectSlots()
{
  this->disconnect(this);
  if(mem_)
    mem_->disconnect(this);
}

void MemoryTreeView::initDragDrop()
{
  setSelectionMode(QAbstractItemView::SingleSelection);
  this->setDragEnabled(true);
  this->setAcceptDrops(true);
  this->setDropIndicatorShown(true);
  setDragDropMode(QAbstractItemView::InternalMove);
}

void MemoryTreeView::setup()
{

}

void MemoryTreeView::contextMenuEvent(QContextMenuEvent *pe)
{
  emit contextMenuPopup();

  m_pmenu->exec(pe->globalPos());
}



void MemoryTreeView::on_itemCollapsed(const QModelIndex &item)
{
  auto *me = static_cast<MEWrapper*>(item.internalPointer());
  QString path = me->getPath();

  expanded_.removeOne(path);
}

void MemoryTreeView::on_itemExpanded(const QModelIndex &item)
{
  auto *me = static_cast<MEWrapper*>(item.internalPointer());
  QString path = me->getPath();

  if(!expanded_.contains(path))
    expanded_ << path;
}

MemoryWrapper *MemoryTreeView::getMem() const
{
  return mem_;
}

void MemoryTreeView::setMem(MemoryWrapper *mem)
{
  if(mem && mem != mem_)
  {
    if(mem_)
      mem_->disconnect(this);

    mem_ = mem;
    //auto model = this->findChild<QMemoryModel*>("model");
    if(model_)
    {
      model_->setMem(mem);
      //this->setModel(nullptr);
      this->setModel(model_);

      auto selectionModel = findChild<QMemorySelectionModel*>("selectionModel");
      if(selectionModel)
      {
        selectionModel->setMem(mem_);
        selectionModel->setModel(model_);
        this->setSelectionModel(selectionModel);
      }

      this->header()->resizeSection(0, 200);

    }

    this->setCurrentIndex(model_->getIndexByMe(mem_->selected()));

    expandItems();
  }
}

void MemoryTreeView::expandItems()
{
  if(!mem_)
    return;

  for(auto path: expanded_)
  {
    MEWrapper *me = mem_->get(path);
    if(me)
    {
      QModelIndex mi = model_->getIndexByMe(me);
      this->setExpanded(mi, true);
    }
  }
}

void MemoryTreeView::loadExpandItems(MEWrapper *me)
{
  if(!me)
    return;
  expanded_.clear();
  expanded_ = me->val().toString().split("\n");

  expandItems();
}

void MemoryTreeView::saveExpandItems(MEWrapper *me)
{
  if(!me)
    return;
  QVariant val = expanded_.join("\n");
  me->setVal(val);
}

MemoryWrapper *MemoryTreeView::memHeader() const
{
  return(model_->getHeaderInfo());
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


