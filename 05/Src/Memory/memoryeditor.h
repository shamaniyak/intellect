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
#include "memorywrapper.h"

class QUndoStack;

class MemoryEditor : public QObject
{
  Q_OBJECT
public:
  explicit MemoryEditor(QObject *parent = nullptr);

  MemoryWrapper *getMem() const;
  void setMem(MemoryWrapper *mem);

signals:

public slots:
  void add(MEWrapper &parent, const QString &name, bool checkExist = true);
  void addFrom(MEWrapper &parent, MEWrapper &mefrom, bool recurs);
  void del(const QString &path);
  void deleteMe(MEWrapper &me);
  void setName(MEWrapper &me, const QString &name);
  void setVal(MEWrapper &me, const QVariant &val);
  void clear();
  void clearMe(const MEWrapper &me);
  void move(MEWrapper &me, MEWrapper &parent, int pos);

private:
  MemoryWrapper *mem_ = nullptr;
  QUndoStack *stack_ = nullptr;

};

#endif // MEMORYEDITOR_H
