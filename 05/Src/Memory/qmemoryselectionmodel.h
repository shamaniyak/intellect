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

#ifndef QMEMORYSELECTIONMODEL_H
#define QMEMORYSELECTIONMODEL_H

#include <QItemSelectionModel>

#include "memorywrapper.h"

class QAbstractItemModel;

class QMemorySelectionModel : public QItemSelectionModel
{
  Q_OBJECT

public:
  explicit QMemorySelectionModel(QAbstractItemModel *model = Q_NULLPTR);
  explicit QMemorySelectionModel(QAbstractItemModel *model, QObject *parent);
  virtual ~QMemorySelectionModel();

  MemoryWrapper *mem() const;
  void setMem(MemoryWrapper *mem);

private slots:
  void on_currentChanged(const QModelIndex &current, const QModelIndex &previous);
  void on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
  void onMemoryChanged(const ChangeEvent &ev);

private:
  MemoryWrapper *mem_ = nullptr;

  void init();
  void setSelected(const MEWrapper &me);
  QModelIndex getIndexByMe(const MEWrapper &me);
  MEWrapper getMeByIndex(const QModelIndex &index);
};

#endif // QMEMORYSELECTIONMODEL_H
