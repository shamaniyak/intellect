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

#ifndef MEMORYCOMPAREPROXYMODEL_H
#define MEMORYCOMPAREPROXYMODEL_H

#include <QObject>
#include <QAbstractProxyModel>

#include "memoryglobal.h"

class MemoryWrapper;
class MEWrapper;
class QMemoryModel;

class MEMORY_EXPORT MemoryCompareProxyModel : public QAbstractProxyModel
{
  Q_OBJECT
  Q_PROPERTY(FilterType filter READ filter WRITE setFilter)
public:

  // Тип фильтрации элементов
  enum FilterType
  {
    NoFilter,           // Без фильтрации
    Exist,              // Существуют в обоих моделях
    NotExist,           // Не существует (удален)
    NotEqualValue       // В одном и том же элементе данные отличаются
  };
  Q_ENUM(FilterType)

  explicit MemoryCompareProxyModel(QObject *parent = nullptr);

  MemoryWrapper *srcMem() const;
  void setSrcMem(MemoryWrapper *srcMem);

  FilterType filter() const;
  void setFilter(const FilterType &filter);

signals:

public slots:

private:
  // Указатель на текущую память, которую сравнивать
  MemoryWrapper *curMem_ = nullptr;
  // Указатель на память, с которой сравнивать
  MemoryWrapper *srcMem_ = nullptr;
  // Результирующая память
  QMemoryModel *resultMem_ = nullptr;
  //QMemoryModel *resultMemoryModel_ = nullptr;
  // Тип фильтрации
  FilterType filter_ = NoFilter;

  // QAbstractProxyModel interface
public:
  QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
  QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

  // QAbstractItemModel interface
public:
  QModelIndex index(int row, int column, const QModelIndex &parent) const override;
  QModelIndex parent(const QModelIndex &child) const override;
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;

private:
  bool checkChanges(MEWrapper &me) const;
  bool checkChangesRecurs(MEWrapper &me) const;
  MEWrapper getMeByIndex(const QModelIndex &index) const;
  void addFrom(MEWrapper &meFrom, MEWrapper &meTo);
};

#endif // MEMORYCOMPAREPROXYMODEL_H
