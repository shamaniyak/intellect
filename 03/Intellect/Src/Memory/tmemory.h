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
  virtual ~IMemory(){}
  virtual void clear() = 0;
};

// Базовый класс по созданию элементов.
// Содержит верхний элемент дерева
class TAbstractMemory : public IMemory
{
public:
  explicit TAbstractMemory();
  virtual ~TAbstractMemory();

  void clear() override;

  TME* get_me();

  TME *selected() const;
  void setSelected(TME *selected);

  bool changed() const;
  void setChanged(bool changed);

  bool can_change() const;
  void set_can_change(bool canChange);

  TME * createNew(TME *parent=nullptr, int count = 1);

protected:
  TME *top_me_ = nullptr;

  //  TAbstractMemory(const TAbstractMemory& src);
//  TAbstractMemory& operator=(const TAbstractMemory& src);

  void lockForRead(TME *me);
  void lockForWrite(TME *me);
  void unlock(TME *me);

private:
  TME *selected_ = nullptr;
  bool changed_ = false;
  bool can_change_ = true;
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

  QString file_path() const;
  void setFile_path(const QString &file_path);

  QStringList& words();

  bool autosave() const;
  void setAutosave(bool autosave);

  // Добавить элемент памяти.
  // Если успешно, то указатель
  TME* add(TME *parent=nullptr, const QString &name="");
  bool add_from(TME *parent, TME *mefrom, bool recurs);
  // Удалить элемент памяти
  bool del(const QString &path);
  //
  bool edit(TME *me, const QString &new_name, QVariant new_val);
  //
  TME* get(const QString &path);
  //
  TME* get_subelement(TME *mep, const QString &name);
  //
  QString get_element_path(TME *get_me) const;
  // получить слово по индексу
  QString get_word(int idx) const;
  // Возвращает индекс слова в списке, если оно есть. Иначе индекс нового добавленного.
  int get_word_idx(const QString &w);
  //
  bool move_element(TME *parent, TME *me, int idx=-1);

  bool open(const QString &fileName);

  bool save();

  bool save_to(const QString &fileName);

  TME* operator[](const QString &path);

  bool undo();
  bool redo();

  bool canUndo();
  bool canRedo();

protected:
//  TMemory(const TMemory& src);
//  TMemory& operator=(const TMemory& src);

  void init();

  void CreateTopME();

  TME* add(const QString &path);

  bool load_memory();

  void save_backup();

  bool add_from_recurse(TME *parent, TME *mefrom);

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
