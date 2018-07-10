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

MemoryCompareProxyModel::MemoryCompareProxyModel(QObject *parent) : QAbstractProxyModel(parent)
{

}

MemoryWrapper *MemoryCompareProxyModel::srcMem() const
{
  return srcMem_;
}

void MemoryCompareProxyModel::setSrcMem(MemoryWrapper *srcMem)
{
  srcMem_ = srcMem;
  Q_ASSERT(srcMem_);

  if(!curMem_)
  {
    auto model = qobject_cast<MemoryWrapper*>(sourceModel());
    Q_ASSERT(model);
    curMem_ = model;
  }

  if(!resultMem_) {
    resultMem_ = new QMemoryModel(this);
  }

  resultMem_->clear();
  resultMem_->addFrom(resultMem_->getME(), curMem_->getME(), true);
  resultMem_->addFrom(resultMem_->getME(), srcMem_->getME(), true, true);
  resultMem_->setColumnCount(2);

  setSourceModel(resultMem_);
}

MemoryCompareProxyModel::FilterType MemoryCompareProxyModel::filter() const
{
  return filter_;
}

void MemoryCompareProxyModel::setFilter(const FilterType &filter)
{
  filter_ = filter;
}

QModelIndex MemoryCompareProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
  if(!proxyIndex.isValid())
    return QModelIndex();
  //auto me = getMeByIndex(proxyIndex);
  //auto meP = me.parent();
  return sourceModel()->index(proxyIndex.row(), proxyIndex.column(),
                              mapToSource(proxyIndex.parent()));
}

QModelIndex MemoryCompareProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
  if(!sourceIndex.isValid())
    return QModelIndex();
  auto me = getMeByIndex(sourceIndex);
  //auto meP = me.parent();
  return createIndex(sourceIndex.row(), sourceIndex.column(), me.getMe());
}

QModelIndex MemoryCompareProxyModel::index(int row, int column, const QModelIndex &parent) const
{
  return mapFromSource(sourceModel()->index(row, column, parent));
}

QModelIndex MemoryCompareProxyModel::parent(const QModelIndex &child) const
{
  return mapFromSource(sourceModel()->parent(child));
}

int MemoryCompareProxyModel::rowCount(const QModelIndex &parent) const
{
  // Подсчитать количество элементов с учетом удаленных
  auto count = sourceModel()->rowCount(mapToSource(parent));



  return count;
}

int MemoryCompareProxyModel::columnCount(const QModelIndex &parent) const
{
  return sourceModel()->columnCount(mapToSource(parent));
}

QVariant MemoryCompareProxyModel::data(const QModelIndex &index, int role) const
{
  if(role == Qt::BackgroundRole && index.isValid())
  {
    auto me = getMeByIndex(index);
    auto path = me.getPath();
    auto me1 = srcMem_->get(path);
    auto me2 = curMem_->get(path);
    // Если такого элемента не было, значит добавлен
    if(!me1) {
      QBrush brush(Qt::green);
      return QVariant(brush);
    }
    // если был, а теперь нету, значит удален
    else if(!me2)
    {
      QBrush brush(Qt::red);
      return QVariant(brush);
    }
    // если изменилось значение
    else if(me.val() != me1.val())
    {
      QBrush brush(Qt::cyan);
      return QVariant(brush);
    }
    else {
      if(checkChangesRecurs(me)) {
        QBrush brush(Qt::yellow, Qt::FDiagPattern);
        return QVariant(brush);
      }
    }
  }

  return QAbstractProxyModel::data(index, role);
}

bool MemoryCompareProxyModel::checkChangesRecurs(MEWrapper &me) const
{
  int i = 0;
  auto me1 = me.getByI(i);
  while(me1) {
    auto path = me1.getPath();
    auto me2 = srcMem_->get(path);
    auto me3 = curMem_->get(path);
    // Если добавлен, удален или изменилось значение, вернуть true, иначе заходим внутрь
    if(!me2 || !me3)
      return true;
    // если изменилось значение, вернуть true
    else if(me1.val() != me2.val())
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
