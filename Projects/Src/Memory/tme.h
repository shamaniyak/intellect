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

#ifndef TME_H
#define TME_H

#include <QMap>
#include <vector>
#include <memory>
#include "tmevalue.h"
#include "memoryglobal.h"

namespace Memory
{
class TAbstractMemory;
class TMemory;

struct TMEData
{
  QString name_;
  QVariant val_;
};

// Базовый элемент
// Для создания элемента верхнего уровня используйте TopME
class MEMORY_EXPORT TME
{
public:

  typedef QMap<int,TME*> elements_map;
  typedef std::vector<TME*> elements_vec;
  typedef std::shared_ptr<TME> shared_me;
  typedef std::weak_ptr<TME> weak_me;
  typedef std::vector<shared_me> vec_shared_me;

  struct MEMORY_EXPORT Elements
  {
    Elements();
    void add(const shared_me &me);
    int count() const;
    shared_me get(int i) const;
    shared_me get_by_id_name(int id) const;
    bool remove(const shared_me me);
    void clear();
    int get_index(const shared_me me) const;
    int get_index(const TME* me) const;
    bool move(int from, int to);

    void load(QDataStream &ds, shared_me parent);
    void save(QDataStream &ds) const;

  private:
    vec_shared_me items_;
  };

  TME();
  TME(shared_me parent, int id_name=-1, QVariant val=0);
  TME(const TME &me);
  ~TME();

  TME& operator =(const TME &me);

  QString name() const;
  void setName(const QString &name);

  QVariant val() const;
  void setVal(const TMEValue &val);
  void setVal(const QVariant &val);

  shared_me parent() const;
  void setParent(shared_me parent);

  QString path() const;

  void add(const shared_me &me);
  bool addFrom(shared_me mefrom, shared_me me_to, bool recurs, bool checkExist = false);
  shared_me get(const QString &name);
  bool del(const QString &name);
  bool del(const shared_me &me);

  void clear();

  int getIndex() const;

  bool move_to(int pos=-1);

  void load(QDataStream &ds);
  void save(QDataStream &ds) const;

  int id_name() const;
  void setIdName(int id);

  Elements& getElements();

  TMemory *mem() const;

  TMEData data() const;

  static int size();

  static shared_me create(shared_me parent, int id=-1, QVariant val=QVariant());

protected:

private:
  int id_name_ = -1;// Любой айди или айди имени в общем массиве слов
  TMEValue val_;
  weak_me parent_;// = nullptr;
  Elements childs_;
};

// Верхний элемент памяти.
// Cлужит для хранения указателя на TMemory.
// Внутренние элементы рекурсивно вызывают родителя до самого верхнего (у которого нет родителя).
// Это нужно для экономии памяти.
class TopME : public TME
{
public:
    typedef std::shared_ptr<TopME> shared_top_me;

  TopME() {}
  TopME(TMemory * mem) : mem_(mem)
  {

  }
  TMemory *mem() const { return mem_; }
  void setMem(TMemory *m) { mem_ = m; }
private:
  TMemory *mem_ = nullptr;
};

}

Q_DECLARE_METATYPE(Memory::TME*)

#endif // ME_H
