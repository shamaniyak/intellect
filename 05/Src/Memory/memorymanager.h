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

#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <QObject>
#include <QMap>
#include "memorywrapper.h"

class MemoryManager : public QObject
{
  Q_OBJECT
public:
  explicit MemoryManager(QObject *parent = 0);
  ~MemoryManager();

  static MemoryWrapper *globalMemory();

signals:

public slots:
  MemoryWrapper *Add(const QString &name);
  MemoryWrapper *Get(const QString &name);
  void Del(const QString &name);

private:
  static MemoryWrapper *globalMemory_;
  static int countLinksToGlobalMemory_;

};

#endif // MEMORYMANAGER_H
