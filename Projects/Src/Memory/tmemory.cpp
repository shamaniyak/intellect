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

#include "tmemory.h"
#include <QIODevice>
#include <QFile>
#include <QDir>
#include <QDateTime>

namespace Memory
{

TAbstractMemory::TAbstractMemory()
{
}

TAbstractMemory::~TAbstractMemory()
{

}

void TAbstractMemory::clear()
{
  QMutexLocker mtxlock(&mtx_);

  selected_ = top_me_;
  top_me_->clear();

  changed_ = true;
}

//TAbstractMemory::TAbstractMemory(const TAbstractMemory &src):
//  top_me_(nullptr), selected_(nullptr), on_change_(nullptr),
//  changed_(false), can_change_(true)
//{
//  *this = src;
//}

//TAbstractMemory &TAbstractMemory::operator=(const TAbstractMemory &src)
//{
////  selected_ = top_me_;
////  if(top_me_){
////    top_me_->clear();
////  }
//  return *this;
//}





TME::shared_me TAbstractMemory::createNew(TME::shared_me parent, int count)
{
  if(!parent)
    parent = top_me_;
  TME::shared_me me;
  for(int i = 0; i < count; ++i) {
    me = TME::create(parent);
  }
  return me;
}

void TAbstractMemory::lockForRead(TME *me)
{
  QMutexLocker mtxlock(&mtx_);

  em_.lockForRead(me);
}

void TAbstractMemory::lockForWrite(TME *me)
{
  QMutexLocker mtxlock(&mtx_);

  em_.lockForWrite(me);
}

void TAbstractMemory::unlock(TME *me)
{
  QMutexLocker mtxlock(&mtx_);

  em_.unlock(me);
}

TME::shared_me TAbstractMemory::getTopME() const
{
  return top_me_;
}

bool TAbstractMemory::getChanged() const
{
  return changed_;
}

void TAbstractMemory::setChanged(bool changed)
{
  changed_ = changed;
}

TME::shared_me TAbstractMemory::getSelected() const
{
  return selected_.lock();
}

void TAbstractMemory::setSelected(const TME::shared_me &selected)
{
  selected_ = selected;
}

//  TMemory

TMemory::TMemory(): TAbstractMemory()
{
  init();
}

TMemory::TMemory(const QString &name): TAbstractMemory(),
  name_(name)
{
  init();
}

TMemory::TMemory(const QString &path, const QString &name): TAbstractMemory(),
  name_(name), file_path_(path)
{
  init();
}

TMemory::~TMemory()
{
  if(getChanged() && autosave_)
    save();
}

void TMemory::init()
{
  CreateTopME();
  createBackup();
}

void TMemory::CreateTopME()
{
  top_me_ = std::make_shared<TopME>(this);
  setSelected(top_me_);
}

void TMemory::clear()
{
  TAbstractMemory::clear();
  id_names_.clear();
  words_.clear();
}

void TMemory::createBackup()
{
  backup_ = std::make_shared<Backup>(this);
  backup_->init();
}

//TMemory::TMemory(const TMemory &src):
//  TAbstractMemory(src)
//{
//  *this = src;
//}

//TMemory &TMemory::operator=(const TMemory &src)
//{
//  return *this;
//}

QString TMemory::getName(TME *me) const
{
  auto it = id_names_.find(me);
  if(it !=id_names_.end())
    return getWord(*it);
  return "";
}

void TMemory::setName(TME *me, const QString &name)
{
  if(!me)
    return;
  if(!id_names_.contains(me))
    id_names_.insert(me, getWordIdx(name));
  else
    id_names_[me] = getWordIdx(name);
}

QString TMemory::getFilePath() const
{
  return file_path_;
}

void TMemory::setFilePath(const QString &path)
{
  if(file_path_ == path)
    return;
  file_path_ = path;
  createBackup();
  setChanged(true);
}

QStringList &TMemory::words()
{
  return words_;
}

bool TMemory::getAutosave() const
{
  return autosave_;
}

void TMemory::setAutosave(bool val)
{
  autosave_ = val;
}

TME::shared_me TMemory::add(const QString &path)
{
  //TME *me = nullptr;

  if(path.isEmpty())
    return add();

//  if(map_me_.find(path) ==map_me_.end())
//  {

//    int count = path.count('\\');
//    QString parentPath = path.section("\\", 0, count -1);
//    QString name = path.section("\\", -1, -1);

//    auto parent = get(parentPath);
//    map_me_.insert(path, add(parent, name));
//  }

  return {};
}

TME::shared_me TMemory::add(TME::shared_me parent, const QString &name)
{
  TME::shared_me me;

  if(!parent)
    parent = getTopME();

  me = createNew(parent);
  me->setName(name);
  //parent->add(me);

  return me;
}

bool TMemory::addFrom(TME::shared_me parent, TME::shared_me mefrom, bool recurs, bool checkExist)
{
  if(!mefrom)
    return false;
  if(!parent)
    parent = getTopME();

  return parent->addFrom(mefrom, parent, recurs, checkExist);
}

bool TMemory::del(const QString &path)
{
  auto me = get(path);

  if(!me)
    return false;

  if(me->parent())
  {
    return me->parent()->del(me);
  }

  return false;
}

TME::shared_me TMemory::get(const QString &path)
{
  if(path.isEmpty())
    return top_me_;

  auto path_ = path.split(QRegExp("[\\\\/]"), QString::SkipEmptyParts);
  auto me = getTopME();
  for(const auto &s: path_)
  {
    me = me->get(s);
    if(!me)
      return me;
  }

  return me;
}

TME::shared_me TMemory::getSubelement(TME::shared_me mep, const QString &name)
{
  if(!mep)
    mep = getTopME();
  return mep->get(name);
}

QString TMemory::getElementPath(TME::shared_me me) const
{
  if(!me)
    return QString();
  else
  {
    QString res = getElementPath(me->parent());
    if(!res.isEmpty())
      res += "\\";
    res += me->name();
    return res;
  }
}

QString TMemory::getWord(int idx) const
{
  if(idx >=0 && idx <words_.size())
    return words_[idx];
  return QString();
  //auto w = id_strings_.key(idx);
  //return w;
}

int TMemory::getWordIdx(const QString &w)
{
//  auto it = std::find(std::begin(words_), std::end(words_), w);
//  if(it != std::end(words_))
//    return std::distance(std::begin(words_), it);

  if(w.isEmpty() || w == "<>")
    return -1;

//  const QRegExp rx(w, Qt::CaseSensitive, QRegExp::Wildcard);
  //QRegularExpression rx(QRegularExpression::escape(w));
  int idx = -1;//words_.indexOf(rx);
  for(int i = 0; i < words_.size(); ++i)
    if(words_[i] == w)
    {
      idx = i; break;
    }

  if(idx >=0)
    return idx;

  words_.push_back(w);
  return words_.size() -1;

//  auto it = id_strings_.find(w);
//  if(it != id_strings_.end())
//    return (*it);

//  unsigned int id = id_strings_.last() +1;
//  id_strings_.insert(w, id);

//  return id;
}

bool TMemory::moveElement(TME::shared_me parent, TME::shared_me me, int idx)
{
  if(!parent || !me || idx < 0)
    return false;
  if(parent != me->parent())
  {
    // удалить из текущего владельца
    if(me->parent())
      me->parent()->del(me);
    //добавить в конец
    me->setParent(parent);
    parent->add(me);
  }
  //переместиться в позицию
  return me->move_to(idx);
}

bool TMemory::open(const QString &fileName)
{
  bool res = false;
  if(getChanged()) {
    if(autosave_) {
      res = save();
      if(!res)
        return res;//не сохранено почему то
    }
  }

  setFilePath(fileName);
  res = loadMemory();

  setChanged(false);

  return res;
}

bool TMemory::save()
{
  saveBackup();

  QFile file(file_path_);
  file.remove();
  if(!file.open(QIODevice::WriteOnly))
    return false;

  file.seek(0);
  QDataStream out(&file);

  quint8 ver = 1;

  out << ver;

  out << words_;

  getTopME()->save(out);

  // сохранить текущий
  QString path;
  if(getSelected())
    path = getSelected()->path();

  out << path;

  setChanged(false);

  return true;
}

bool TMemory::saveTo(const QString &fileName)
{
  if(fileName.isEmpty())
    return false;
  setFilePath(fileName);
  return save();
}

TME::shared_me TMemory::operator[](const QString &path)
{
  return get(path);
}

bool TMemory::undo()
{
  // Прежде чем отменить, нужно сохранить
  if(getChanged() && autosave_)
    if(!save())
      return false;

  if(backup_.get())
    return backup_->undo();

  return false;
}

bool TMemory::redo()
{
  if(backup_.get())
    return backup_->redo();

  return false;
}

bool TMemory::canUndo()
{
  return backup_->canUndo();
}

bool TMemory::canRedo()
{
  return backup_->canRedo();
}

bool TMemory::loadMemory()
{
  QFile file(file_path_);
  if(!file.open(QIODevice::ReadOnly))
    return false;

  file.seek(0);
  QDataStream in(&file);

  quint8 ver = 0;

  in >> ver;

  words_.clear();
  in >> words_;

  getTopME()->clear();
  getTopME()->load(in);
  getTopME()->getElements().load(in, getTopME());

  // установить текущий
  auto me = getTopME();
  if(ver >0)
  {
    QString path;
    in >> path;

    me = get(path);
    if(!me)
      me = getTopME();
  }
  setSelected(me);

  return true;
}

void TMemory::saveBackup()
{
    if(!backup_)
        createBackup();
  if(backup_)
    backup_->save();
}

Backup::Backup(TMemory *m) : mem_(m)
{
  init();
}

void Backup::init()
{
  setPathAndName();
  loadFileList();
}

void Backup::save()
{
  if(!mem_ || path_.isEmpty())
    return;
  // если мы в конце списка, то значит мы тут с помощью redo(), и сохранять не надо
  //    if(current_ == files_.size()-1)
  //      return;

  QDir dir(path_);
  if(!dir.exists())
    if(!dir.mkpath(path_))
      return;

  // добавим дату и время к имени
  QDateTime dt = QDateTime::currentDateTime();
  QString new_name = name_ + dt.toString("yyyy.MM.dd hh.mm.ss") + ".moi";
  QString new_fn = path_ + new_name;

  QFile::copy(mem_->getFilePath(), new_fn);

  files_ << new_name;

  //++current_;
  current_ = files_.size();
}

bool Backup::undo()
{
  if(!canUndo())
    return false;//отменить не возможно
  // если мы в конце списка, то сделать бэкап текущей
  if(current_ == files_.size())
  {
    save();
    --current_;
  }
  // предыдущий
  --current_;

  return loadFile();
}

bool Backup::redo()
{
  // если мы в конце списка, то не чего возвращать
  if(!canRedo())
    return false;

  ++current_;

  return loadFile();
}

bool Backup::canUndo()
{
  //нечего отменять, некуда отменять, нету больше
  if(files_.empty() || !mem_ || current_ ==0)
    return false;

  return true;
}

bool Backup::canRedo()
{
  if(current_ >= files_.size()-1)
    return false;
  return true;
}

void Backup::setPathAndName()
{
  path_ = QString();
  if(!mem_)
    return;

  QFile file(mem_->getFilePath());
  if(!file.exists())
    return;

  QFileInfo fileInfo(file);

  name_ = fileInfo.fileName();
  path_ = fileInfo.absolutePath();
  path_ += "/bkp/" + name_ + "/";
}

void Backup::loadFileList()
{
  QDir dir(path_);
  if(dir.exists())
  {
    files_ = dir.entryList(QDir::Files);
  }
  current_ = files_.size();
}

bool Backup::loadFile()
{
  auto file = path_ + files_[current_];
  if(QFile::remove(mem_->getFilePath()))
  {
    if(QFile::copy(file, mem_->getFilePath()))
      return mem_->loadMemory();
  }

  return false;
}

}
