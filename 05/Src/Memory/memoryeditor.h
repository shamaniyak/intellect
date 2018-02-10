#ifndef MEMORYEDITOR_H
#define MEMORYEDITOR_H

#include <QObject>
#include "memorywrapper.h"

class QUndoStack;

class MemoryEditor : public QObject
{
  Q_OBJECT
public:
  explicit MemoryEditor(QObject *parent = nullptr);

  MemoryWrapper *getMem() const;
  void setMem(MemoryWrapper *mem);

signals:

public slots:
  void add(MEWrapper *parent, const QString &name, bool checkExist = true);
  void addFrom(MEWrapper *parent, MEWrapper *mefrom, bool recurs);
  void del(const QString &path);
  void deleteMe(MEWrapper *me);
  void setName(MEWrapper *me, const QString &name);
  void setVal(MEWrapper *me, const QVariant &val);
  void clear();
  void clearMe(MEWrapper *me);
  void move(MEWrapper *me, MEWrapper *parent, int pos);

private:
  MemoryWrapper *mem_ = nullptr;
  QUndoStack *stack_ = nullptr;

};

#endif // MEMORYEDITOR_H
