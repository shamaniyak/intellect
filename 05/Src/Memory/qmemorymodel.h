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

#ifndef QMEMORYMODEL_H
#define QMEMORYMODEL_H

#include "memorywrapper.h"

class QMemoryModel : public MemoryWrapper
{
  Q_OBJECT

  typedef MemoryWrapper inherited;
public:
  explicit QMemoryModel(QObject *parent = 0);
  ~QMemoryModel();

signals:

public slots:
  int getColumnCount() const;
  void setColumnCount(int columnCount);

  // QAbstractItemModel interface
public:
  QModelIndex index(int row, int column, const QModelIndex &parent) const override;
  QModelIndex parent(const QModelIndex &child) const override;

  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;

  QVariant data(const QModelIndex &index, int role) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role) override;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) override;

  bool canFetchMore(const QModelIndex &parent) const override;
  void fetchMore(const QModelIndex &parent) override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;

  bool hasChildren(const QModelIndex &parent) const override;

  Qt::DropActions  supportedDragActions() const override;
  Qt::DropActions supportedDropActions() const override;

  QHash<int,QByteArray> roleNames() const override;

  // QAbstractItemModel interface
public:
  QStringList mimeTypes() const override;
  QMimeData *mimeData(const QModelIndexList &indexes) const override;
  bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
  bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
  bool insertRows(int row, int count, const QModelIndex &parent) override;

private:
  int columnCount_ = 1;
  QStringList headers = {tr("Name"), tr("Value"), tr("Path"), tr("Type")};
  enum MyRoles
  {
    ValueRole = Qt::UserRole + 1
  };

};

#endif // QMEMORYMODEL_H
