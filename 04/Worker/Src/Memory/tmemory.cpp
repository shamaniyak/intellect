#include "tmemory.h"
#include <QIODevice>
#include <QFile>
#include <QDir>

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





TME *TAbstractMemory::createNew(TME *parent, int count)
{
  if(!parent)
    parent = top_me_;
  TME *me = nullptr;
  for(int i = 0; i < count; ++i)
    me = parent->Add("");
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

TME *TAbstractMemory::getTopME()
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

TME *TAbstractMemory::getSelected() const
{
  return selected_;
}

void TAbstractMemory::setSelected(TME *selected)
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

  //autosave_ = !path.isEmpty();

  createBackup();
}

TMemory::~TMemory()
{
  if(getChanged() && autosave_)
    save();
  setSelected(nullptr);
  delete (TopME*)top_me_;
}

void TMemory::init()
{
  CreateTopME();
}

void TMemory::CreateTopME()
{
  top_me_ = new TopME(this);
  setSelected(top_me_);
}

void TMemory::clear()
{
  TAbstractMemory::clear();
  id_names_.clear();
}



void TMemory::createBackup()
{
  backup_.reset();
  backup_ = std::make_shared<Backup>(this);
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

void TMemory::setfilePath(const QString &path)
{
  file_path_ = path;
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

TME *TMemory::add(const QString &path)
{
  TME *me = nullptr;

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

  return me;
}

TME *TMemory::add(TME *parent, const QString &name)
{
  TME *me = nullptr;

  if(!parent)
    parent = getTopME();

  //me = createNew(parent);

//  if(!name.isEmpty())
//    setName(me, name);
//  if(me)
//    me->setName(name);

  //if(parent)
  {
    //me = parent->Get(name);
    //if(!me)
      me = parent->Add(name);
  }

  return me;
}

bool TMemory::addFromRecurse(TME *parent, TME *mefrom)
{
  if(!parent || !mefrom)
    return false;
  auto elements = mefrom->getElements();
  for(int i =0; i <elements.count(); ++i)
  {
    auto me1 = elements.get(i);
    auto me2 = add(parent, me1->name());//parent->Add(me1->name());
    if(me2)
    {
      me2->setVal(me1->val());

      addFromRecurse(me2, me1);
    }
  }

  return true;
}

bool TMemory::addFrom(TME *parent, TME *mefrom, bool recurs)
{
  bool res = false;
  if(!mefrom)
    return res;
  if(!parent)
    parent = getTopME();

  return parent->addFrom(mefrom, recurs);

//  auto elements = mefrom->getElements();
//  for(int i =0; i <elements.count(); ++i)
//  {
//    auto me1 = elements.get(i);
//    //auto me2 = parent->Get(me1->name());
//    auto me2 = add(parent, me1->name());// parent->Add(me1->name());
//    if(me2)
//    {
//      me2->setVal(me1->val());

//      if(recurs)
//        addFromRecurse(me2, me1);
//    }
//  }

//  res = true;

//  return res;
}

bool TMemory::del(const QString &path)
{
  auto me = get(path);

  if(!me)
    return false;

  if(me->parent())
  {
    return me->parent()->Del(me->name());
  }

  return false;
}

bool TMemory::edit(TME *me, const QString &new_name, QVariant new_val)
{
  if(!me)
    return false;

  me->setName(new_name);
  me->setVal(new_val);

//  do_change(me, EMemoryChange::mcEdt);

  return true;
}

TME *TMemory::get(const QString &path)
{
  if(path.isEmpty())
    return nullptr;

  //path.remove()
  auto path_ = path.split(QRegExp("[\\\\/]"), QString::SkipEmptyParts);
  auto me = getTopME();
  for(const auto &s: path_)
  {
    me = me->Get(s);
    if(!me)
      return me;
  }

//  if(map_me_.find(path) != map_me_.end())
//    return map_me_[path];

  return me;
}

TME *TMemory::getSubelement(TME *mep, const QString &name)
{
  if(!mep)
    mep = getTopME();
  return mep->Get(name);
}

QString TMemory::getElementPath(TME *me) const
{
  if(!me)
    return "";
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
  return "";
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

  int idx = words_.indexOf(QRegExp(w, Qt::CaseSensitive));
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

bool TMemory::moveElement(TME *parent, TME *me, int idx)
{
  if(!parent || !me)
    return false;
  bool res = me->move_to(parent, idx);
//  do_change(me, EMemoryChange::mcNone);
  return res;
}

bool TMemory::open(const QString &fileName)
{
  bool res = false;
  if(getChanged() && autosave_) {
    res = save();
    if(!res)
      return res;//не сохранено почему то
  }

  file_path_ = fileName;
  res = loadMemory();

  createBackup();

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
    path = getSelected()->getPath();

  out << path;

  setChanged(false);

  return true;
}

bool TMemory::saveTo(const QString &fileName)
{
  if(fileName.isEmpty())
    return false;
  setfilePath(fileName);
  return save();
}

TME *TMemory::operator[](const QString &path)
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

  // установить текущий
  TME *me = getTopME();
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
//  if(!backup_.get())
//    backup_ = std::make_shared<Backup>(this);

  if(backup_.get())
    backup_->save();

//  QFile file(file_path_);
//  if(!file.exists())
//    return;

//  QFileInfo fileInfo(file);
//  QString name = fileInfo.fileName();
//  QString path = fileInfo.absolutePath();
//  path += "/bkp/";

//  QDir dir(path);
//  if(!dir.exists())
//    if(!dir.mkdir(path))
//      return;

//  // добавим дату и время к имени
//  QDateTime dt = QDateTime::currentDateTime();
//  QString new_fn = path + name + dt.toString("yyyy.MM.dd hh.mm.ss") + ".moi";

//  QFile::copy(file_path_, new_fn);
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
    if(!dir.mkdir(path_))
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
