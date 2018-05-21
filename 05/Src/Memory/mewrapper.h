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

  friend class MemoryWrapper;
};

Q_DECLARE_METATYPE(MEWrapper)

#endif // MEWRAPPER_H
