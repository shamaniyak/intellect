#ifndef MEWRAPPER_H
#define MEWRAPPER_H

#include <QObject>
#include <QVariant>

namespace Memory
{
  class TME;
}

class MemoryWrapper;

class MEWrapper
{
  friend class MemoryWrapper;
public:
  MEWrapper();
  explicit MEWrapper(MemoryWrapper *mem);
  explicit MEWrapper(Memory::TME *me, MemoryWrapper *mem = 0);
  ~MEWrapper();

  void clear();

  Memory::TME *getMe() const;

  MemoryWrapper *getMem() const;

  QString name() const;
  void setName(const QString &name);

  QVariant val() const;
  void setVal(const QVariant &val);

  QString getPath() const;

  MEWrapper *add(const QString &name, bool checkExist = true);
  bool addFrom(MEWrapper *from, bool recurs = true);

  MEWrapper *get(const QString &name);
  MEWrapper *getByI(int i);

  void del(const QString &name);
  void delByI(int i);
  void delByMe(MEWrapper *me);

  MEWrapper *parent() const;

  int count() const;

  int getIndex() const;

  bool deleted() const;

protected:
  void setMem(MemoryWrapper *mem);
  void deleteMe(MEWrapper *me);

private:
  Memory::TME *me_ = 0;
  MemoryWrapper *mem_;

  void clearR(Memory::TME *me);
};

//Q_DECLARE_METATYPE(MEWrapper)
Q_DECLARE_METATYPE(MEWrapper*)

#endif // MEWRAPPER_H
