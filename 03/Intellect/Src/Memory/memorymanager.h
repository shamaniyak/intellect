#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <QObject>
#include <QMap>
#include "memorywrapper.h"

class MemoryManager : public QObject
{
  Q_OBJECT
public:
  explicit MemoryManager(QObject *parent = 0);

signals:

public slots:
  MemoryWrapper *Add(const QString &name);
  MemoryWrapper *Get(const QString &name);
  void Del(const QString &name);

private:
  //QMap<QString, TMemory*> map_mems_;
  QMap<QString, MemoryWrapper*> map_mw_;
};

#endif // MEMORYMANAGER_H
