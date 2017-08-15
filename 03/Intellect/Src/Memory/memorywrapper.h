#ifndef MEMORYWRAPPER_H
#define MEMORYWRAPPER_H

// Обертка для Memory.
// * Используется для скриптов, реализует сигналы и слоты
// * для многопоточного взаимодействия
// *


#include <QObject>
#include <memory>
#include <QMap>

#include "tmemory.h"
#include "mewrapper.h"
//#include "qmemorymodel.h"

namespace Memory
{
  class TMemory;
  class TME;
}

class MemoryLogger;// Объявлен дальше

enum EMemoryChange {
  mcNone, mcAdd, mcAddFrom, mcDel, mcEditName, mcEditVal, mcUpdate, mcSelect, mcClear, mcMove
};

class MemoryWrapper : public QObject//QMemoryModel
{
  Q_OBJECT
  Q_PROPERTY(QString file_path READ file_path WRITE setFile_path)
  Q_PROPERTY(MEWrapper *me READ getME)
  Q_PROPERTY(MEWrapper* selected READ selected WRITE setSelected)
  Q_PROPERTY(bool canChange READ canChange WRITE setCanChange)

public:

  struct ChangeEvent
  {
    EMemoryChange type = mcNone;
    MEWrapper *me = nullptr;
    MEWrapper *parent = nullptr;
    int row = 0;
    int count = 0;
  };

  explicit MemoryWrapper(QObject *parent = 0);
  ~MemoryWrapper();

  std::shared_ptr<MemoryLogger> getLogger();

  MEWrapper *CreateMEW(Memory::TME *me);
  void DeleteMEW(Memory::TME *me);

signals:
  // посылается перед удалением
  //before_change(MEWrapper *me, EMemoryChange idMsg);
  on_change(MEWrapper *me, EMemoryChange idMsg);
  change(const ChangeEvent &ev);

public slots:
  MEWrapper *getME();

  void addCount(MEWrapper *parent, int count = 1);
  MEWrapper* add(MEWrapper *parent, const QString &name);
  bool addFrom(MEWrapper *parent, MEWrapper *mefrom, bool recurs);
  // Удалить элемент памяти
  void del(const QString &path);

  MEWrapper *get(const QString &path);
  MEWrapper *getById(uint id);

  bool autosave() const;
  void setAutosave(bool autosave);

  bool open(const QString &fileName);
  bool save();

  QString file_path() const;
  void setFile_path(const QString &file_path);

  QVariant getVal(const QString &path);

  void setSelected(MEWrapper *me);
  MEWrapper *selected();

  void doChange(MEWrapper *me, EMemoryChange idMsg);

  void clear();

  void move(MEWrapper *me, MEWrapper *parent, int pos);

  bool canChange() const;
  void setCanChange(bool val);

  bool changed() const;

  void undoBackup();
  void redoBackup();

  bool canUndoBackup();
  bool canRedoBackup();

protected:
  void clearDeleted();
  void clearMeWrappers();

private:
  typedef QMap<Memory::TME*, MEWrapper*> t_mapMeWrappers;
  typedef std::vector<MEWrapper*> t_vecMeWrappers;

  std::shared_ptr<Memory::TMemory> mem_;
  t_mapMeWrappers map_mew_;  // Обертки над TME
  std::shared_ptr<MemoryLogger> logger_;
  t_vecMeWrappers deleted_; // Список удаленных

  friend class MEWrapper;
};

// Предназначен для хранения команд обращений к памяти
class MemoryLogger
{
public:

  MemoryLogger(MemoryWrapper *m) : mem_(m)
  {
    init();
  }

  void init()
  {

  }

  void save()
  {

  }

  bool undo()
  {
    return false;
  }

  bool redo()
  {
    return false;
  }

  bool canUndo()
  {
    return false;
  }

  bool canRedo()
  {
    return false;
  }

protected:


private:
  MemoryWrapper *mem_;

  int current_ = -1;
};

//
Q_DECLARE_METATYPE(MemoryWrapper*)

#endif // MEMORYWRAPPER_H
