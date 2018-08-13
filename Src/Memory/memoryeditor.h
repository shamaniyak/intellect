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

#ifndef MEMORYEDITOR_H
#define MEMORYEDITOR_H

#include <QObject>
#include "qmemorymodel.h"

class QUndoStack;

class MemoryEditor : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QMemoryModel* mem READ getMem WRITE setMem NOTIFY memChanged)
  Q_PROPERTY(QObject* stack READ getStack)
  Q_PROPERTY(bool canRedo READ canRedo NOTIFY canRedoChanged)
  Q_PROPERTY(bool canUndo READ canUndo NOTIFY canUndoChanged)
public:
  explicit MemoryEditor(QObject *parent = nullptr);

  QMemoryModel *getMem() const;
  void setMem(QMemoryModel *mem);

  QObject *getStack() const;

signals:

  void memChanged(QMemoryModel* mem);

  void canUndoChanged(bool canUndo);
  void canRedoChanged(bool canRedo);

public slots:
  void add(const MEWrapper &parent, const QString &name, bool checkExist = true);
  void addFrom(MEWrapper &parent, const MEWrapper &mefrom, bool recurs);
  void del(const QString &path);
  void deleteMe(MEWrapper me);
  void setName(const MEWrapper &me, const QString &name);
  void setVal(const MEWrapper &me, const QVariant &val);
  void clear();
  void clearMe(const MEWrapper &me);
  void move(MEWrapper &me, MEWrapper &parent, int pos);
  void undo();
  void redo();
  bool canUndo();
  bool canRedo();

private:
  mutable QMemoryModel *mem_ = nullptr;
  QUndoStack *stack_ = nullptr;
};

#endif // MEMORYEDITOR_H
