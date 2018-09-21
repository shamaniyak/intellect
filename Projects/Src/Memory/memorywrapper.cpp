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

#include "memorywrapper.h"
#include "tmemory.h"
#include "memorymanager.h"

#include <QDebug>

//  MemoryWrapper

MemoryWrapper::MemoryWrapper(QObject *parent) : QAbstractItemModel(parent),
  mem_(new Memory::TMemory())
{
  //qDebug() << Memory::TME::size();
		//mew_.reserve(999983);
}

MemoryWrapper::~MemoryWrapper()
{
  clearDeleted();
  clearMeWrappers();
}

MEWrapper MemoryWrapper::add(const MEWrapper &parent, const QString &name, bool checkExist)
{
  //qDebug() << "MemoryWrapper::add begin";
  MEWrapper me;
  auto p = parent;
  // если парент не задан, то парентом будет корневой элемент
  if(p.isNull())
    p = this->getME();
  if(p)
  {
    // проверка на существоваие с тем же именем
    if(!name.isEmpty() && checkExist)
      me = p.get(name);

    if(me.isNull())
    {
      // создаем, если не существует
      auto parentIndex = getIndexByMe(p);
      int row = p.count();
      beginInsertRows(parentIndex, row, row);

      me = CreateMEW(mem_->add(p.getMe(), name));

      endInsertRows();

      if(me) {
        doChange(me, EMemoryChange::mcAdd);
      }
    }
  }
  //qDebug() << "MemoryWrapper::add end";
  return me;
}

void MemoryWrapper::add(const MEWrapper &parent, const MEWrapper &me)
{
  if(!me)
    return;
  auto p = parent;
  if(!p)
    p = getME();
  if(p) {
    auto parentIndex = getIndexByMe(p);
    int row = p.count();
    beginInsertRows(parentIndex, row, row);

    p.getMe()->add(me.getMe());

    endInsertRows();

    if(me) {
      doChange(me, EMemoryChange::mcAdd);
    }
  }
}

bool MemoryWrapper::addFrom(const MEWrapper &parent, const MEWrapper &mefrom, bool recurs, bool checkExist)
{
  bool  res = false;
  // если парент не задан, то парентом будет корневой элемент
  auto p = parent;
  if(p.isNull())
    p = getME();

  if(p && mefrom && mefrom.count() > 0)
  {
    auto parentIndex = getIndexByMe(p);
    int first = p.count();
    int last = first + mefrom.count() - 1;
    beginInsertRows(parentIndex, first, last);

    bool res = mem_->addFrom(p.getMe(), mefrom.getMe(), recurs, checkExist);

    endInsertRows();

    if(res)
      doChange(parent, EMemoryChange::mcAddFrom);
  }

  return res;
}

void MemoryWrapper::del(const QString &path)
{
  auto me = get(path);
  deleteMe(me);
}

void MemoryWrapper::deleteMe(const MEWrapper &me)
{
  //qDebug() << "MemoryWrapper::deleteMe begin";
  if(me.isNull() || me.parent().isNull())
    return;

  ChangeEvent ev;
	ev.type = mcDel;
  ev.me = me;
  ev.parent = me.parent();
  ev.row = me.getIndex();
  ev.path = me.getPath();
  //ev.count = me->count();

  auto parentIndex = getIndexByMe(ev.parent);
  beginRemoveRows(parentIndex, ev.row, ev.row);

  auto me1 = me.getMe();
  clearR(me1);
  ev.parent.getMe()->del(me1);

  endRemoveRows();

  doChange(ev);
  //qDebug() << "MemoryWrapper::deleteMe end";
}

MEWrapper MemoryWrapper::getME() const
{
  return const_cast<MemoryWrapper*>(this)->CreateMEW(mem_->getTopME());
}

void MemoryWrapper::addCount(const MEWrapper &parent, int count)
{
  if(count <= 0)
    return;
  auto p = parent;
  if(!p)
     p = getME();

  auto parentIndex = getIndexByMe(p);
  int first = p.count();
  int last = first + count - 1;
  beginInsertRows(parentIndex, first, last);

  mem_->createNew(p.getMe(), count);

  endInsertRows();

  doChange(parent, mcAddFrom);
}

MEWrapper MemoryWrapper::get(const QString &path)
{
  return CreateMEW(mem_->get(path));
}

MEWrapper MemoryWrapper::getById(uint id)
{
	//auto me = reinterpret_cast<Memory::TME*>(id);
	//auto me = Memory::TME::getById(id);
	//MEWrapper resMe(me, this);
	MEWrapper resMe = mew_.value(id, MEWrapper());
  return resMe;
}

bool MemoryWrapper::getAutosave() const
{
  return mem_->getAutosave();
}

void MemoryWrapper::setAutosave(bool autosave)
{
  mem_->setAutosave(autosave);
}

bool MemoryWrapper::open(const QString &fileName)
{
  bool result = false;
  beginResetModel();

  result = mem_->open(fileName);

  endResetModel();

  if(result)
    setSelected(getSelected());

  return result;
}

bool MemoryWrapper::save()
{
  if(mem_->getChanged())
    return mem_->save();
  return false;
}

QString MemoryWrapper::getFilePath() const
{
  return mem_->getFilePath();
}

void MemoryWrapper::setFilePath(const QString &file_path)
{
  mem_->setFilePath(file_path);
  emit filePathChanged(file_path);
}

QVariant MemoryWrapper::getVal(const QString &path)
{
  auto me = get(path);
  if(me)
    return me.val();

  return QVariant();
}

void MemoryWrapper::setVal(const MEWrapper &me, const QVariant &val)
{
  if(me)
  {
    if(val == me.val())
      return;

		ChangeEvent ev;
		ev.type = mcEditVal;
		ev.me = me;
		ev.prevVal = me.val();

		me.getMe()->setVal(val);

		if(!canChange_)
			return;

			ev.parent = me.parent();
			ev.path = me.getPath();
		ev.row = me.getIndex();

    auto index = getIndexByMe(me);
    dataChanged(index, index, {ValueColumn});

    doChange(ev);
    emit valueChanged(me);
  }
}

void MemoryWrapper::setName(const MEWrapper &me, const QString &name)
{
  if(me)
  {
    if(name == me.name())
      return;

    ChangeEvent ev;
		ev.type = mcEditName;
    ev.me = me;
    ev.prevName = me.name();

    me.me_->setName(name);

    ev.path = me.getPath();

    auto index = getIndexByMe(me);
    dataChanged(index, index, {NameColumn});

    doChange(ev);
  }
}

void MemoryWrapper::setSelected(const MEWrapper &me)
{
  MEWrapper me1;
  if(me) me1 = me;
  else me1 = getME();
  if(mem_->getSelected() != me1.getMe()) {
    mem_->setSelected(me1.getMe());

    doChange(me1, EMemoryChange::mcSelect);
    emit selectedChanged();
  }
}

MEWrapper MemoryWrapper::getSelected()
{
  return CreateMEW(mem_->getSelected());
}

QModelIndex MemoryWrapper::getIndexByMe(const MEWrapper &me)
{
  if(me.isNull())
    return QModelIndex();

  if(me == getME())
    return QModelIndex();

  return createIndex(me.getIndex(), 0, me.getUid());
}

MEWrapper MemoryWrapper::getMeByIndex(const QModelIndex &index) const
{
  auto id = reinterpret_cast<uint>(index.internalPointer());
  return const_cast<MemoryWrapper*>(this)->getById(id);
}

void MemoryWrapper::doChange(const MEWrapper &me, EMemoryChange idMsg)
{
  if(!me) return;

  bool changed = idMsg !=EMemoryChange::mcSelect && idMsg != EMemoryChange::mcUpdate;
  if(changed)
  {
    mem_->setChanged(changed);

    //if(mem_->autosave())
      //mem_->save();//будем сохранять каждое изменение
  }

  if(canChange_) {

    ChangeEvent ev;
    ev.type = idMsg;
    ev.me = me;
    ev.parent = me.parent();
    ev.row = me.getIndex();
    ev.count = me.count();
    ev.path = me.getPath();
    emit change(ev);
  }
}

void MemoryWrapper::doChange(const ChangeEvent &ev)
{
  bool changed = ev.type !=EMemoryChange::mcSelect && ev.type != EMemoryChange::mcUpdate;
  if(changed)
  {
    mem_->setChanged(changed);

    //if(mem_->autosave())
      //mem_->save();//будем сохранять каждое изменение
  }

  if(canChange_) {
    //emit on_change(ev.me, ev.type);

    emit change(ev);
  }
}

void MemoryWrapper::clear()
{
  getME().setVal(QVariant());
  setSelected(getME());
  clearMe(getME());
}

void MemoryWrapper::clearMe(const MEWrapper &me)
{
  if(me && me.count() > 0) {
    auto index = getIndexByMe(me);
    beginRemoveRows(index, 0, me.count()-1);

    clearME1(me.getMe());
    me.getMe()->clear();

    endRemoveRows();

    ChangeEvent ev;
    ev.type = EMemoryChange::mcClear;
    ev.me = me;
    ev.parent = me.parent();
    ev.row = me.getIndex();
    ev.count = me.count();
    ev.path = me.getPath();
    doChange(ev);
  }
}

void MemoryWrapper::clearR(Memory::TME::shared_me me)
{
  clearME1(me);
  DeleteMEW(me);
}

void MemoryWrapper::clearME1(Memory::TME::shared_me me)
{
  auto childs = me->getElements();
  int cnt = childs.count();
  for(int i = 0; i <cnt; ++i)
  {
    clearR(childs.get(i));
  }
  //me->clear();
}

bool MemoryWrapper::move(const MEWrapper &me, const MEWrapper &parent, int pos)
{
  if(me && parent) {
    // запрещаем перенос из одного владельца в другого
    if(me.parent() != parent)
      return false;
    if(pos < 0 || pos >= parent.count())
      return false;

    auto source = getIndexByMe(parent);
//    if(!source.isValid())
//      return false;
    int row = me.getIndex();
    int destRow = pos > row ? pos + 1: pos;
    if(row==destRow)
      return false;
    beginMoveRows(source, row, row, source, destRow);

    bool ok = mem_->moveElement(parent.getMe(), me.getMe(), pos);

    endMoveRows();

    if(ok)
      doChange(me, mcMove);

    return ok;
  }

  return false;
}

bool MemoryWrapper::getCanChange() const
{
  return canChange_;
}

void MemoryWrapper::setCanChange(bool val)
{
  canChange_ = val;
}

bool MemoryWrapper::changed() const
{
  return mem_->getChanged();
}

void MemoryWrapper::undoBackup()
{
  beginResetModel();

  if(mem_->undo())
  {
    mem_->setChanged(false);
    doChange(getME(), EMemoryChange::mcUpdate);
  }

  endResetModel();
}

void MemoryWrapper::redoBackup()
{
  beginResetModel();

  if(mem_->redo())
  {
    mem_->setChanged(false);
    doChange(getME(), EMemoryChange::mcUpdate);
  }

  endResetModel();
}

bool MemoryWrapper::canUndoBackup()
{
  return mem_->canUndo();
}

bool MemoryWrapper::canRedoBackup()
{
  return mem_->canRedo();
}

void MemoryWrapper::clearDeleted()
{
  foreach (auto me, deleted_) {
    delete me;
  }

  deleted_.clear();
}

void MemoryWrapper::clearMeWrappers()
{
  mew_.clear();
}

MEWrapper MemoryWrapper::CreateMEW(const Memory::TME::shared_me &me)
{
    //static int createMEWCnt = 0;
  //qDebug() << "MemoryWrapper::CreateMEW" << me.get() << me.use_count();
  if(!me)
    return MEWrapper();
	//qDebug() << me->id_;
	//return MEWrapper(me, this);

	uint id = uint(me.get());
	if(!mew_.contains(id))
		mew_[id] = MEWrapper(me, this);

	return mew_[id];
}

void MemoryWrapper::DeleteMEW(Memory::TME::shared_me me)
{
  if(!me)
    return;

	uint id = uint(me.get());
	if(mew_.contains(id)) {
		mew_.remove(id);
	}
}
