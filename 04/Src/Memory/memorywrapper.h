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

class QUndoStack;

enum EMemoryChange {
  mcNone, mcAdd, mcAddFrom, mcDel, mcEditName, mcEditVal, mcUpdate, mcSelect, mcClear, mcMove
};

struct ChangeEvent
{
private:
  Q_GADGET
  Q_PROPERTY(int row MEMBER row)
public:
  EMemoryChange type = mcNone;
  MEWrapper *me = nullptr;
  MEWrapper *parent = nullptr;
  int row = 0;
  int count = 0;
  int first = 0;
  int last = 0;
  QString prevName;
  QVariant prevVal;


};

class MemoryWrapper : public QObject//QMemoryModel
{
  Q_OBJECT
  Q_PROPERTY(MEWrapper *me READ getME)
  Q_PROPERTY(QString filePath READ getFilePath WRITE setFilePath)
  Q_PROPERTY(MEWrapper* selected READ getSelected WRITE setSelected)
  Q_PROPERTY(bool canChange READ getCanChange WRITE setCanChange)
  Q_PROPERTY(bool autosave READ getAutosave WRITE setAutosave)

public:

  explicit MemoryWrapper(QObject *parent = 0);
  ~MemoryWrapper();

  MEWrapper *getME();

  MEWrapper *CreateMEW(Memory::TME *me);
  void DeleteMEW(Memory::TME *me);

  void doChange(const ChangeEvent &ev);

  bool getAutosave() const;
  void setAutosave(bool autosave);

  bool getCanChange() const;
  void setCanChange(bool val);

  QString getFilePath() const;
  void setFilePath(const QString &filePath);

  void setSelected(MEWrapper *me);
  MEWrapper *getSelected();

  void createUndoStack();
  QUndoStack *getStack();

signals:
  // посылается перед удалением
  //before_change(MEWrapper *me, EMemoryChange idMsg);
  on_change(MEWrapper *me, EMemoryChange idMsg);
  change(const ChangeEvent &ev);
  change1(const Memory::TME &ev);

public slots:

  void addCount(MEWrapper *parent, int count = 1);
  MEWrapper* add(MEWrapper *parent, const QString &name, bool checkExist = true);
  bool addFrom(MEWrapper *parent, MEWrapper *mefrom, bool recurs);
  // Удалить элемент памяти
  void del(const QString &path);
  void deleteMe(MEWrapper *me);

  MEWrapper *get(const QString &path);
  MEWrapper get1(const QString &path);
  MEWrapper *getById(uint id);

  bool open(const QString &fileName);
  bool save();

  QVariant getVal(const QString &path);
  void setVal(MEWrapper *me, const QVariant &val);

  void setName(MEWrapper *me, const QString &name);

  void doChange(MEWrapper *me, EMemoryChange idMsg);

  void clear();
  void clearMe(MEWrapper *me);

  bool move(MEWrapper *me, MEWrapper *parent, int pos);

  bool changed() const;

  void undoBackup();
  void redoBackup();

  bool canUndoBackup();
  bool canRedoBackup();

protected:
  void clearDeleted();
  void clearMeWrappers();

  MEWrapper* add1(MEWrapper *parent, const QString &name, bool checkExist = true);
  bool addFrom1(Memory::TME *parent, Memory::TME *mefrom, bool recurs);
  void deleteMe1(MEWrapper *me);
  void clearR(Memory::TME *me);
  void clearMe1(MEWrapper *me);
  bool move1(MEWrapper *me, MEWrapper *parent, int pos);
  void setVal1(MEWrapper *me, const QVariant &val);
  void setName1(MEWrapper *me, const QString &name);

private:
  typedef QMap<Memory::TME*, MEWrapper*> t_mapMeWrappers;
  typedef QMultiMap<QString, MEWrapper*> t_multiMapMeWrappers;
  typedef std::vector<MEWrapper*> t_vecMeWrappers;

  std::shared_ptr<Memory::TMemory> mem_;
  t_mapMeWrappers map_mew_;  // Обертки над TME
  // Каждому имени сопоставлен список элементов
  t_multiMapMeWrappers elements_;
  t_vecMeWrappers deleted_; // Список удаленных
  QUndoStack *stack_ = nullptr;
  bool canChange_ = true;

  friend class QMemoryModel;
  friend class MEWrapper;
  friend class AddCommand;
  friend class DelCommand;
  friend class AddFromCommand;
  friend class ClearCommand;
  friend class MoveCommand;
  friend class EditNameCommand;
  friend class EditValCommand;
};
//
Q_DECLARE_METATYPE(MemoryWrapper*)
Q_DECLARE_METATYPE(ChangeEvent)
Q_DECLARE_METATYPE(MEData)

#endif // MEMORYWRAPPER_H
