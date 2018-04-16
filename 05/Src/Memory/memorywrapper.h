#ifndef MEMORYWRAPPER_H
#define MEMORYWRAPPER_H

// ������� ��� Memory.
// * ������������ ��� ��������, ��������� ������� � �����
// * ��� �������������� ��������������
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
  MEWrapper me;
  MEWrapper parent;
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
  Q_PROPERTY(MEWrapper me READ getME)
  Q_PROPERTY(QString filePath READ getFilePath WRITE setFilePath)
  Q_PROPERTY(MEWrapper selected READ getSelected WRITE setSelected)
  Q_PROPERTY(bool canChange READ getCanChange WRITE setCanChange)
  Q_PROPERTY(bool autosave READ getAutosave WRITE setAutosave)

public:

  explicit MemoryWrapper(QObject *parent = 0);
  ~MemoryWrapper();

  // �������� �������
  MEWrapper getME();

  MEWrapper CreateMEW(Memory::TME *me);
  void DeleteMEW(Memory::TME *me);

  void doChange(const ChangeEvent &ev);

  bool getAutosave() const;
  void setAutosave(bool autosave);

  bool getCanChange() const;
  void setCanChange(bool val);

  QString getFilePath() const;
  void setFilePath(const QString &filePath);

  void setSelected(const MEWrapper &me);
  MEWrapper getSelected();

signals:
  void on_change(const MEWrapper &me, EMemoryChange idMsg);
  void change(const ChangeEvent &ev);
  void change1(const Memory::TME &ev);

public slots:

  void addCount(const MEWrapper &parent, int count = 1);
  MEWrapper add(const MEWrapper &parent, const QString &name, bool checkExist = true);
  bool addFrom(const MEWrapper &parent, const MEWrapper &mefrom, bool recurs, bool checkExist = false);
  // ������� ������� ������
  void del(const QString &path);
  void deleteMe(const MEWrapper &me);

  MEWrapper get(const QString &path);
  MEWrapper getById(uint id);

  bool open(const QString &fileName);
  bool save();

  QVariant getVal(const QString &path);
  void setVal(const MEWrapper &me, const QVariant &val);

  void setName(const MEWrapper &me, const QString &name);

  void doChange(const MEWrapper &me, EMemoryChange idMsg);

  void clear();
  void clearMe(const MEWrapper &me);
  //void deleteChildren

  bool move(const MEWrapper &me, const MEWrapper &parent, int pos);

  bool changed() const;

  void undoBackup();
  void redoBackup();

  bool canUndoBackup();
  bool canRedoBackup();

protected:
  void clearDeleted();
  void clearMeWrappers();

  void clearR(Memory::TME *me);
  void clearME1(Memory::TME *me);

private:
  typedef QMap<Memory::TME*, MEWrapper> t_mapMeWrappers;
  typedef QMultiMap<QString, MEWrapper*> t_multiMapMeWrappers;
  typedef std::vector<MEWrapper*> t_vecMeWrappers;

  std::shared_ptr<Memory::TMemory> mem_;
  t_mapMeWrappers map_mew_;  // ������� ��� TME
  // ������� ����� ����������� ������ ���������
  t_multiMapMeWrappers elements_;
  t_vecMeWrappers deleted_; // ������ ���������
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

#endif // MEMORYWRAPPER_H
