#include "qmemoryselectionmodel.h"
#include "qmemorymodel.h"

QMemorySelectionModel::QMemorySelectionModel(QAbstractItemModel *model)
  : QItemSelectionModel(model)
{
  init();
}

QMemorySelectionModel::QMemorySelectionModel(QAbstractItemModel *model, QObject *parent)
  : QItemSelectionModel(model, parent)
{
  init();
}

QMemorySelectionModel::~QMemorySelectionModel()
{

}

void QMemorySelectionModel::init()
{
  connect(this, &QItemSelectionModel::currentChanged,
          this, &QMemorySelectionModel::on_currentChanged);
//  connect(this, &QItemSelectionModel::selectionChanged,
//          this, &QMemorySelectionModel::on_selectionChanged);
}

MemoryWrapper *QMemorySelectionModel::mem() const
{
  return mem_;
}

void QMemorySelectionModel::setMem(MemoryWrapper *mem)
{
  if(mem_)
    mem_->disconnect(this);

  mem_ = mem;

  if(mem_)
  {
    setModel(mem_);
    setSelected(mem_->getSelected());
  }
}

void QMemorySelectionModel::on_currentChanged(const QModelIndex &current, const QModelIndex &/*previous*/)
{
  if(mem_)
  {
//    mem_->disconnect(this);
    mem_->setSelected(getMeByIndex(current));
//    connect(mem_, &MemoryWrapper::on_change,
//            this, &QMemorySelectionModel::memory_change);
  }
}

void QMemorySelectionModel::on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
  if(mem_)
  {
    int cnt = selected.size();
    auto index = selected.indexes()[cnt-1];
    uint id = reinterpret_cast<uint>(index.internalPointer());
    mem_->setSelected(mem_->getById(id));
  }
}

void QMemorySelectionModel::setSelected(const MEWrapper &me)
{
  QModelIndex index = getIndexByMe(me);

  setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
}

QModelIndex QMemorySelectionModel::getIndexByMe(const MEWrapper &me)
{
  auto m = qobject_cast<MemoryWrapper*>(model());
  if(m)
  {
    return m->getIndexByMe(me);
  }
  return QModelIndex();
}

MEWrapper QMemorySelectionModel::getMeByIndex(const QModelIndex &index)
{
  auto m = qobject_cast<MemoryWrapper*>(model());
  if(m)
  {
    return m->getMeByIndex(index);
  }
  return MEWrapper();
}
