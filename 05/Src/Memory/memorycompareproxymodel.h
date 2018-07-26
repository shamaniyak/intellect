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
#include <QSortFilterProxyModel>

#include "memoryglobal.h"

class MemoryWrapper;
class MEWrapper;
class QMemoryModel;

class MEMORY_EXPORT MemoryCompareProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT
  Q_PROPERTY(FilterType filter READ filter WRITE setFilter)
    Q_PROPERTY(QMemoryModel* sourceMemory READ sourceMemory WRITE setSourceMemory NOTIFY sourceMemoryChanged)
    Q_PROPERTY(QMemoryModel* compareMemory READ compareMemory WRITE setCompareMemory NOTIFY compareMemoryChanged)
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

  QMemoryModel *sourceMemory() const;
  void setSourceMemory(QMemoryModel *mem);

  QMemoryModel *compareMemory() const;
  void setCompareMemory(QMemoryModel *mem);

  FilterType filter() const;
  void setFilter(const FilterType &filter);

  // QAbstractProxyModel interface
public:
//  QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
//  QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

  // QAbstractItemModel interface
public:
//  QModelIndex index(int row, int column, const QModelIndex &parent) const override;
//  QModelIndex parent(const QModelIndex &child) const override;
//  int rowCount(const QModelIndex &parent) const override;
//  int columnCount(const QModelIndex &parent) const override;
  Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

signals:
  void sourceMemoryChanged();
  void compareMemoryChanged();

public slots:
  bool compare();

private:
  enum RoleEx {
      VisibleRole = Qt::UserRole + 1
  };

  bool checkChanges(MEWrapper &me) const;
  bool checkChangesRecurs(MEWrapper &me) const;
  MEWrapper getMeByIndex(const QModelIndex &index) const;
  void addFrom(MEWrapper &meFrom, MEWrapper &meTo);
  // Указатель на текущую память, которую сравнивать
  QMemoryModel *curMem_ = nullptr;
  // Указатель на память, с которой сравнивать
  QMemoryModel *compareMem_ = nullptr;
  // Результирующая память
  QMemoryModel *resultMem_ = nullptr;
  //QMemoryModel *resultMemoryModel_ = nullptr;
  // Тип фильтрации
  FilterType filter_ = NoFilter;

  // QSortFilterProxyModel interface
protected:
  bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

#endif // MEMORYCOMPAREPROXYMODEL_H
