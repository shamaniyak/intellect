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
          this, &QMemorySelectionModel::on_itemSelectionChanged);
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
    setSelected(mem_->getSelected());

    connect(mem_, &MemoryWrapper::on_change,
            this, &QMemorySelectionModel::memory_change);
  }
}

void QMemorySelectionModel::memory_change(MEWrapper *me, EMemoryChange idMsg)
{
  if(!me)
    return;

  switch(idMsg)
  {
    case EMemoryChange::mcSelect:
      setSelected(me);
      break;
    case EMemoryChange::mcDel:
      break;

    case EMemoryChange::mcNone:
    case EMemoryChange::mcAdd:
    case EMemoryChange::mcEditName:
    case EMemoryChange::mcEditVal:
    case EMemoryChange::mcClear:
    case EMemoryChange::mcAddFrom:
    case EMemoryChange::mcUpdate:
    case mcMove:
      break;
  }
}

void QMemorySelectionModel::on_itemSelectionChanged(const QModelIndex &current, const QModelIndex &/*previous*/)
{
  if(mem_)
  {
//    mem_->disconnect(this);
    mem_->setSelected((MEWrapper*)current.internalPointer());
//    connect(mem_, &MemoryWrapper::on_change,
//            this, &QMemorySelectionModel::memory_change);
  }
}

void QMemorySelectionModel::setSelected(MEWrapper *me)
{
  QModelIndex index = getIndexByMe(me);
//  auto me1 = static_cast<MEWrapper*>(index.internalPointer());
//  auto name = me->name();
//  auto name1 = me1->name();

  setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
  //select(index,
  //       QItemSelectionModel::ClearAndSelect);
}

QModelIndex QMemorySelectionModel::getIndexByMe(MEWrapper *me)
{
  auto m = qobject_cast<QMemoryModel*>(model());
  if(m)
  {
    return m->getIndexByMe(me);
  }
  return QModelIndex();
}
