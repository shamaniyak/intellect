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

#ifndef MEMORYWRAPPER_H
#define MEMORYWRAPPER_H

// Обертка для Memory.
// * Используется для скриптов, реализует сигналы и слоты
// * для многопоточного взаимодействия
// *


#include <QObject>
#include <QAbstractItemModel>
#include <QMap>
#include <memory>

#include "tmemory.h"
#include "mewrapper.h"

namespace Memory
{
  class TMemory;
  class TME;
}

enum EMemoryChange {
  mcNone, mcAdd, mcAddFrom, mcDel, mcEditName, mcEditVal, mcUpdate, mcSelect, mcClear, mcMove
};

struct ChangeEvent
{
private:
  Q_GADGET
  Q_PROPERTY(int type READ getType)
  Q_PROPERTY(int row MEMBER row)
  Q_PROPERTY(QString path MEMBER path)
  Q_PROPERTY(MEWrapper me MEMBER me)
public:
  EMemoryChange type = mcNone;
  MEWrapper me;
  MEWrapper parent;
  int row = 0;
  int count = 0;
  int first = 0;
  int last = 0;
  QString prevName;
  QVariant prevVal;
  QString path;

  Q_ENUM(EMemoryChange)

  int getType() {return type;}
};

class MEMORY_EXPORT MemoryWrapper : public QAbstractItemModel
{
  Q_OBJECT
  Q_PROPERTY(MEWrapper me READ getME)
  Q_PROPERTY(QString filePath READ getFilePath WRITE setFilePath NOTIFY filePathChanged)
  Q_PROPERTY(MEWrapper selected READ getSelected WRITE setSelected NOTIFY selectedChanged)
  Q_PROPERTY(bool canChange READ getCanChange WRITE setCanChange)
  Q_PROPERTY(bool autosave READ getAutosave WRITE setAutosave)

public:

  explicit MemoryWrapper(QObject *parent = 0);
  ~MemoryWrapper();

  // Корневой элемент
  MEWrapper getME() const;

  MEWrapper CreateMEW(const Memory::TME::shared_me &me);
  void DeleteMEW(Memory::TME::shared_me me);

  void doChange(const ChangeEvent &ev);

  bool getAutosave() const;
  void setAutosave(bool autosave);

  bool getCanChange() const;
  void setCanChange(bool val);

  QString getFilePath() const;
  void setFilePath(const QString &filePath);

  void setSelected(const MEWrapper &me);
  MEWrapper getSelected();

  QModelIndex getIndexByMe(const MEWrapper &me);
  Q_INVOKABLE MEWrapper getMeByIndex(const QModelIndex &index) const;

signals:
  void change(const ChangeEvent &ev);
  void selectedChanged();
  void filePathChanged(QString filePath);
  void valueChanged(const MEWrapper &me);

public slots:

  void addCount(const MEWrapper &parent, int count = 1);
  MEWrapper add(const MEWrapper &parent, const QString &name, bool checkExist = true);
  void add(const MEWrapper &parent, const MEWrapper &me);
  bool addFrom(const MEWrapper &parent, const MEWrapper &mefrom, bool recurs, bool checkExist = false);
  // Удалить элемент памяти
  void del(const QString &path);
  void deleteMe(const MEWrapper &me);

  MEWrapper get(const QString &path);
  MEWrapper getById(uint id);

  bool open(const QString &fileName);
  bool save();

  QVariant getVal(const QString &path);
  void setVal(const MEWrapper &me, const QVariant &val);

  void setName(const MEWrapper &me, const QString &name);

  void doChange(const MEWrapper &me, EMemoryChange idMsg);

  void clear();
  void clearMe(const MEWrapper &me);
  //void deleteChildren

  bool move(const MEWrapper &me, const MEWrapper &parent, int pos);

  bool changed() const;

  void undoBackup();
  void redoBackup();

  bool canUndoBackup();
  bool canRedoBackup();

protected:
  enum Columns
  {
    NameColumn,
    ValueColumn,
    PathColumn,

    ColumnCount
  };

  void clearDeleted();
  void clearMeWrappers();

  void clearR(Memory::TME::shared_me me);
  void clearME1(Memory::TME::shared_me me);

private:
  typedef QMap<Memory::TME*, MEWrapper> t_mapMeWrappers;
  typedef QMultiMap<QString, MEWrapper*> t_multiMapMeWrappers;
  typedef std::vector<MEWrapper*> t_vecMeWrappers;

  std::shared_ptr<Memory::TMemory> mem_;
  t_mapMeWrappers map_mew_;  // Обертки над TME
  // Каждому имени сопоставлен список элементов
  t_multiMapMeWrappers elements_;
  t_vecMeWrappers deleted_; // Список удаленных
  bool canChange_ = true;

  friend class QMemoryModel;
  friend class MEWrapper;
  friend class AddCommand;
  friend class DelCommand;
  friend class AddFromCommand;
  friend class ClearCommand;
  friend class MoveCommand;
  friend class EditNameCommand;
  friend class EditValCommand;
  QString m_filePath;
};
//
//Q_DECLARE_METATYPE(MemoryWrapper*)
Q_DECLARE_METATYPE(ChangeEvent)

#endif // MEMORYWRAPPER_H
