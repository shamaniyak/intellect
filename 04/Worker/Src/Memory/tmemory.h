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

// �������� ������
class Backup;

class IMemory
{
public:
  virtual void release() = 0;
  virtual void clear() = 0;
};

// ������� ����� �� �������� ���������.
// �������� ������� ������� ������
class TAbstractMemory : public IMemory
{
public:
  explicit TAbstractMemory();
  virtual ~TAbstractMemory();
  virtual void release() override final { delete this; }

  void clear() override;

  TME* getTopME();

  TME *getSelected() const;
  void setSelected(TME *getSelected);

  bool getChanged() const;
  void setChanged(bool getChanged);

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
  QMutex mtx_;
  ElementsManager em_;
};

// ��������� ���������� ������
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
  void setfilePath(const QString &path);

  QStringList& words();

  bool getAutosave() const;
  void setAutosave(bool val);

  // �������� ������� ������.
  // ���� �������, �� ���������
  TME* add(TME *parent=nullptr, const QString &name="");
  bool addFrom(TME *parent, TME *mefrom, bool recurs);
  // ������� ������� ������
  bool del(const QString &path);
  //
  bool edit(TME *me, const QString &new_name, QVariant new_val);
  //
  TME* get(const QString &path);
  //
  TME* getSubelement(TME *mep, const QString &name);
  //
  QString getElementPath(TME *getTopME) const;
  // �������� ����� �� �������
  QString getWord(int idx) const;
  // ���������� ������ ����� � ������, ���� ��� ����. ����� ������ ������ ������������.
  int getWordIdx(const QString &w);
  //
  bool moveElement(TME *parent, TME *me, int idx=-1);

  bool open(const QString &fileName);

  bool save();

  bool saveTo(const QString &fileName);

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

  bool loadMemory();

  void saveBackup();

  bool addFromRecurse(TME *parent, TME *mefrom);

  void createBackup();

private:
  //! ��� ������
  QString name_;
  //! ���� � ����� �� �����
  QString file_path_;
  //! ������ ������������. ������ ��� �������� ������������� ����
  QStringList words_;
  //! ������� �������������� ��� ����������� (���� ����� ����)
  bool autosave_ = false;
  //! ������ ���������� ����������� ��� ����������
  std::shared_ptr<Backup> backup_;

  // <����, TME*>
  //QMap<QString, TME*> map_me_;
  QMap<TME*, unsigned int> id_names_;
  //QMap<QString, unsigned int> id_strings_;//! ������ ������������. ������ ��� �������� ������������� �����
};


// ����� ��������� ���������� ������.
// ���������� current_ = ���-�� ������.
// ��� ���������� �� ����������� current_;
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
  QStringList files_;//! ������ ������ ������
  QString path_;
  QString name_;
  int current_ = -1;
};


}

#endif // MEMORY_H
