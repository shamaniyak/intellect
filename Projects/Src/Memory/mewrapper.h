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
#include <memory>

#include "memoryglobal.h"
#include "tme.h"

class MemoryWrapper;

// Удобная обертка для работы с TME.
// Нужна для QML.
// Работает через указатель на MemoryWrapper
class MEMORY_EXPORT MEWrapper
{
  Q_GADGET
  Q_PROPERTY(QString name READ name WRITE setName)
  Q_PROPERTY(QVariant val READ val WRITE setVal)
  Q_PROPERTY(QString path READ getPath)
  Q_PROPERTY(int index READ getIndex)
  Q_PROPERTY(int count READ count)
  Q_PROPERTY(uint uid READ getUid)
  Q_PROPERTY(MEWrapper parent READ parent)
  Q_PROPERTY(MemoryWrapper* mem READ getMem)

public:
  MEWrapper();
  MEWrapper(MemoryWrapper *mem);
	MEWrapper(const Memory::TME::shared_me &me, MemoryWrapper *mem = nullptr);
  MEWrapper(const MEWrapper &src);
  ~MEWrapper();

  Memory::TME::shared_me getMe() const;

  MemoryWrapper *getMem() const;

  QString name() const;
  void setName(const QString &name);

  QVariant val() const;
  void setVal(const QVariant &val);

  QString getPath() const;

  Q_INVOKABLE MEWrapper add(const QString &name, bool checkExist = true);
	Q_INVOKABLE bool addFrom(const MEWrapper &from, bool recurs = true);

  Q_INVOKABLE MEWrapper get(const QString &name);
  Q_INVOKABLE MEWrapper getByI(int i);

  Q_INVOKABLE void del(const QString &name);
  Q_INVOKABLE void delByI(int i);
  Q_INVOKABLE void delByMe(const MEWrapper &me);
  Q_INVOKABLE void clear();
  Q_INVOKABLE void move(const MEWrapper &parent, int pos);

  MEWrapper parent() const;

  int count() const;

  int getIndex() const;

  Q_INVOKABLE bool isNull() const;
  Q_INVOKABLE bool isValid() const;

  uint getUid() const;

  explicit operator bool() const { return !isNull(); }
  bool operator !() const { return isNull(); }

  bool operator ==(MEWrapper const& r) const { return getMe() == r.getMe(); }
  bool operator !=(MEWrapper const& r) const { return !(*this == r); }

  MEWrapper &operator =(const MEWrapper &src);

protected:
  void setMem(MemoryWrapper *mem);
  void deleteMe(const MEWrapper &me);

private:
  std::shared_ptr<Memory::TME> me_ = 0;
	MemoryWrapper *mem_ = nullptr;

  friend class MemoryWrapper;
};

Q_DECLARE_METATYPE(MEWrapper)

#endif // MEWRAPPER_H
