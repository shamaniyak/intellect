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

#include "memorycompareproxymodel.h"

#include <Src/Memory/qmemorymodel.h>

#include <QBrush>

MemoryCompareProxyModel::MemoryCompareProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    setRecursiveFilteringEnabled(true);
}

QMemoryModel *MemoryCompareProxyModel::sourceMemory() const
{
    return curMem_;
}

void MemoryCompareProxyModel::setSourceMemory(QMemoryModel *mem)
{
    curMem_ = mem;
}

QMemoryModel *MemoryCompareProxyModel::compareMemory() const
{
  return compareMem_;
}

void MemoryCompareProxyModel::setCompareMemory(QMemoryModel *mem)
{
    if(compareMem_ == mem)
        return;
  compareMem_ = mem;
  emit compareMemoryChanged();
}

MemoryCompareProxyModel::FilterType MemoryCompareProxyModel::filter() const
{
  return filter_;
}

void MemoryCompareProxyModel::setFilter(const FilterType &filter)
{
    filter_ = filter;
}

bool MemoryCompareProxyModel::compare()
{
    if(!compareMem_ || ! curMem_)
        return false;

    if(!resultMem_) {
      resultMem_ = new QMemoryModel(this);
      resultMem_->setColumnCount(2);
    }

    resultMem_->clear();
    resultMem_->addFrom(resultMem_->getME(), curMem_->getME(), true);
    resultMem_->addFrom(resultMem_->getME(), compareMem_->getME(), true, true);

    setSourceModel(resultMem_);

    beginResetModel();
    endResetModel();

    return true;
}

QVariant MemoryCompareProxyModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
  if(role == Qt::DecorationRole)
  {
    QModelIndex srcIndex = mapToSource(index);
    auto me = getMeByIndex(srcIndex);
    auto path = me.getPath();
    auto me1 = curMem_->get(path);
    auto me2 = compareMem_->get(path);
    // Если такого элемента не было, значит добавлен
    if(!me1) {
      QColor brush(Qt::green);
      return QVariant(brush);
    }
    // если был, а теперь нету, значит удален
    else if(!me2)
    {
      QColor brush(Qt::red);
      return QVariant(brush);
    }
    // если изменилось значение
    else if(me2.val() != me1.val())
    {
      QColor brush(Qt::cyan);
      return QVariant(brush);
    }
    else {
      if(checkChangesRecurs(me)) {
        //QBrush brush(Qt::yellow, Qt::FDiagPattern);
        QColor brush(Qt::yellow);
        return QVariant(brush);
      }
    }
  }

  return QSortFilterProxyModel::data(index, role);
}

QHash<int, QByteArray> MemoryCompareProxyModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractProxyModel::roleNames();
    roles[Qt::BackgroundRole] = "background";
    roles[VisibleRole] = "visible";
    return roles;
}

bool MemoryCompareProxyModel::checkChanges(MEWrapper &me) const
{
  // Если добавлен, удален или изменилось значение, вернуть true, иначе false
  auto path = me.getPath();
  auto me2 = compareMem_->get(path);
  auto me3 = curMem_->get(path);
  // Если добавлен, удален или изменилось значение, вернуть true, иначе заходим внутрь
  if(!me2 || !me3)
    return true;
  // если изменилось значение, вернуть true
  else if(me3.val() != me2.val())
    return true;
  return false;
}

bool MemoryCompareProxyModel::checkChangesRecurs(MEWrapper &me) const
{
  if(checkChanges(me))
    return true;
  int i = 0;
  auto me1 = me.getByI(i);
  while(me1) {
    // Если добавлен, удален или изменилось значение, вернуть true, иначе заходим внутрь
    if(checkChanges(me1))
      return true;
    else {
      if(checkChangesRecurs(me1))
        return true;
    }
    me1 = me.getByI(++i);
  }
  return false;
}

MEWrapper MemoryCompareProxyModel::getMeByIndex(const QModelIndex &index) const
{
  uint id = reinterpret_cast<uint>(index.internalPointer());
  return resultMem_->getById(id);
}

void MemoryCompareProxyModel::addFrom(MEWrapper &meFrom, MEWrapper &meTo)
{
  int i = 0;
  auto me = meFrom.getByI(i);
  while(me)
  {
    auto me1 = meTo.get(me.name());
    if(me1)
    {

    }
  }
}

bool MemoryCompareProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QAbstractItemModel *model = sourceModel();
    QModelIndex sourceIndex = model->index(sourceRow, 0, sourceParent);
    if (!sourceIndex.isValid())
      return true;
    auto me = getMeByIndex(sourceIndex);
    bool result = checkChangesRecurs(me);
    return result;
}
