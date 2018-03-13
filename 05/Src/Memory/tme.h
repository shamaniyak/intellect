#ifndef TME_H
#define TME_H

#include <QMap>
#include <vector>
#include "tmevalue.h"

namespace Memory
{
class TAbstractMemory;
class TMemory;

struct TMEData
{
  QString name_;
  QVariant val_;
};

class TME
{
public:

  typedef QMap<int,TME*> elements_map;
  typedef std::vector<TME*> elements_vec;

  struct Elements
  {
    Elements();
    TME * add(int id, TME *parent);
    void add(TME *me);
    int count() const;
    TME *get(int i) const;
    TME *get_by_id_name(int id) const;
    bool remove(const TME *me);
    void clear();
    int get_index(const TME *me) const;
    bool move(int from, int to);

    void load(QDataStream &ds, TME *parent);
    void save(QDataStream &ds) const;

  private:
    elements_vec items_;
  };

  TME();
  TME(TME *parent, int id_name=-1, QVariant val=0);
  TME(const TME &me);
  ~TME();

  TME& operator =(const TME &me);

  QString name() const;
  void setName(const QString &name);

  QVariant val() const;
  void setVal(const TMEValue &val);
  void setVal(const QVariant &val);

  TME *parent() const;
  void setParent(TME *parent);

  QString getPath() const;

  TME *Add(const QString &name);
  bool addFrom(TME *mefrom, bool recurs, bool checkExist = false);
  TME *Get(const QString &name);
  bool Del(const QString &name);
  bool Del(TME *me);

  void clear();

  int get_index() const;

  bool move_to(TME *parent, int pos=-1);

  void load(QDataStream &ds);
  void save(QDataStream &ds) const;

  int id_name() const;

  Elements& getElements();

  TMemory *mem() const;

  TMEData data() const;

protected:
  void remove(const TME *me);

private:
  int id_name_ = -1;// Любой айди или айди имени в общем массиве слов
  TMEValue val_;
  TME *parent_ = nullptr;
  Elements childs_;
};

// Верхний элемент памяти.
// Cлужит для хранения указателя на TMemory.
// Внутренние элементы рекурсивно вызывают родителя до самого верхнего (у которого нет родителя).
// Это нужно для экономии памяти.
class TopME : public TME
{
public:
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
