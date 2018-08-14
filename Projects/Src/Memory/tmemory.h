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

#ifndef TMEMORY_H
#define TMEMORY_H

#include <QObject>
#include <memory>
#include <QVector>
#include <QMap>
#include <QStringList>
#include "elementsmanager.h"

namespace Memory
{

// Объявлен дальше
class Backup;

class IMemory
{
public:
  virtual void release() = 0;
  virtual void clear() = 0;
};

// Базовый класс по созданию элементов.
// Содержит верхний элемент дерева
class TAbstractMemory : public IMemory
{
public:
  explicit TAbstractMemory();
  virtual ~TAbstractMemory();
  virtual void release() override final { delete this; }

  void clear() override;

  TME::shared_me getTopME() const;

  TME::shared_me getSelected() const;
  void setSelected(const TME::shared_me &getSelected);

  bool getChanged() const;
  void setChanged(bool getChanged);

  TME::shared_me createNew(TME::shared_me parent=nullptr, int count = 1);

protected:
  TME::shared_me top_me_ = nullptr;

  //  TAbstractMemory(const TAbstractMemory& src);
//  TAbstractMemory& operator=(const TAbstractMemory& src);

  void lockForRead(TME *me);
  void lockForWrite(TME *me);
  void unlock(TME *me);

private:
  TME::weak_me selected_;// = nullptr;
  bool changed_ = false;
  QMutex mtx_;
  ElementsManager em_;
};

// Реализует именованую память
class TMemory : public TAbstractMemory
{
  friend class Backup;

public:
  explicit TMemory();
  explicit TMemory(const QString &name);
  explicit TMemory(const QString &path, const QString &name);
  ~TMemory();

  void clear() override;

  QString getName(TME *me) const;
  void setName(TME *me, const QString &name);

  QString getFilePath() const;
  void setFilePath(const QString &path);

  QStringList& words();

  bool getAutosave() const;
  void setAutosave(bool val);

  // Добавить элемент памяти.
  // Если успешно, то указатель
  TME::shared_me add(TME::shared_me parent=nullptr, const QString &name="");
  bool addFrom(TME::shared_me parent, TME::shared_me mefrom, bool recurs, bool checkExist=false);
  // Удалить элемент памяти
  bool del(const QString &path);
  //
  TME::shared_me get(const QString &path);
  //
  TME::shared_me getSubelement(TME::shared_me mep, const QString &name);
  //
  QString getElementPath(TME::shared_me getTopME) const;
  // получить слово по индексу
  QString getWord(int idx) const;
  // Возвращает индекс слова в списке, если оно есть. Иначе индекс нового добавленного.
  int getWordIdx(const QString &w);
  //
  bool moveElement(TME::shared_me parent, TME::shared_me me, int idx=-1);

  bool open(const QString &fileName);

  bool save();

  bool saveTo(const QString &fileName);

  TME::shared_me operator[](const QString &path);

  bool undo();
  bool redo();

  bool canUndo();
  bool canRedo();

protected:
//  TMemory(const TMemory& src);
//  TMemory& operator=(const TMemory& src);

  void init();

  void CreateTopME();

  TME::shared_me add(const QString &path);

  bool loadMemory();

  void saveBackup();

  void createBackup();

private:
  //! Имя памяти
  QString name_;
  //! Путь к файлу на диске
  QString file_path_;
  //! Список наименований. Служит для хранения повторяющихся слов
  QStringList words_;
  //! Признак автосохранения при уничтожении (если задан путь)
  bool autosave_ = false;
  //! Объект резервного копирования при сохранении
  std::shared_ptr<Backup> backup_;

  // <Путь, TME*>
  //QMap<QString, TME*> map_me_;
  QMap<TME*, unsigned int> id_names_;
  //QMap<QString, unsigned int> id_strings_;//! Список наименований. Служит для хранения повторяющихся строк
};


// Класс резевного копрования пямяти.
// Изначально current_ = кол-ву файлов.
// При сохранении мы увеличиваем current_;
class Backup
{
public:

  Backup(TMemory *m);

  void init();

  void save();

  bool undo();

  bool redo();

  bool canUndo();

  bool canRedo();

protected:
  void setPathAndName();

  void loadFileList();

  bool loadFile();

private:
  TMemory *mem_;
  QStringList files_;//! Список файлов Бэкапа
  QString path_;
  QString name_;
  int current_ = -1;
};


}

#endif // MEMORY_H
