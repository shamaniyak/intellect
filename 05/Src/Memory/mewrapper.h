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
  Q_GADGET
  Q_PROPERTY(QString name READ name WRITE setName)
  Q_PROPERTY(QString path READ getPath)
  Q_PROPERTY(QVariant val READ val WRITE setVal)
//  Q_PROPERTY(QString path READ getPath)
//  Q_PROPERTY(MEWrapper* parent READ parent)
public:
  MEWrapper();
  MEWrapper(MemoryWrapper *mem);
  MEWrapper(Memory::TME *me, MemoryWrapper *mem = 0);
  MEWrapper(const MEWrapper &src);
  ~MEWrapper();

  void clear();

  Memory::TME *getMe() const;

  MemoryWrapper *getMem() const;

  QString name() const;
  void setName(const QString &name);

  QVariant val() const;
  void setVal(const QVariant &val);

  QString getPath() const;

  MEWrapper add(const QString &name, bool checkExist = true);
  bool addFrom(MEWrapper &from, bool recurs = true);

  MEWrapper get(const QString &name);
  MEWrapper getByI(int i);

  void del(const QString &name);
  void delByI(int i);
  void delByMe(MEWrapper &me);

  MEWrapper parent() const;

  int count() const;

  int getIndex() const;

  bool isNull() const;

  explicit operator bool() const { return !isNull(); }
  bool operator !() const { return isNull(); }

  bool operator ==(MEWrapper const& r) const { return getMe() == r.getMe(); }
  bool operator !=(MEWrapper const& r) const { return !(*this == r); }

protected:
  void setMem(MemoryWrapper *mem);
  void deleteMe(MEWrapper &me);

private:
  Memory::TME *me_ = 0;
  MemoryWrapper *mem_ = 0;

  void clearR(Memory::TME *me);

  friend class MemoryWrapper;
};

Q_DECLARE_METATYPE(MEWrapper)

#endif // MEWRAPPER_H
