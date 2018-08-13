/****************************************************************************
**
** Copyright (C) 2015-2018 Aleksandr Abramov
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
** http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
****************************************************************************/

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
    connect(mem_, &MemoryWrapper::change, this, &QMemorySelectionModel::onMemoryChanged);
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
  Q_UNUSED(deselected)
  if(mem_)
  {
    int cnt = selected.size();
    auto index = selected.indexes()[cnt-1];
    uint id = reinterpret_cast<uint>(index.internalPointer());
    mem_->setSelected(mem_->getById(id));
  }
}

void QMemorySelectionModel::onMemoryChanged(const ChangeEvent &ev)
{
  if(ev.type == mcSelect) {
    setSelected(ev.me);
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
