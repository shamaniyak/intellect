#ifndef MEWRAPPER_H
#define MEWRAPPER_H

#include <QObject>
#include <QVariant>

namespace Memory
{
  class TME;
}

class MemoryWrapper;

class MEWrapper// : public QObject
{
  friend class MemoryWrapper;

//  Q_OBJECT
//  Q_PROPERTY(QString name READ name WRITE setName)
//  Q_PROPERTY(QVariant val READ val WRITE setVal)
//  Q_PROPERTY(QString path READ getPath)
//  Q_PROPERTY(MEWrapper* parent READ parent)
public:
  explicit MEWrapper(MemoryWrapper *mem = 0);
  explicit MEWrapper(Memory::TME *me = 0, MemoryWrapper *mem = 0);
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
