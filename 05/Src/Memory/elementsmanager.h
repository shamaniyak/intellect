#ifndef ELEMENTSMANAGER_H
#define ELEMENTSMANAGER_H

#include <QReadWriteLock>
#include <QMap>
#include "tme.h"

namespace Memory
{

class ElementsManager
{
public:
  ElementsManager();
  ~ElementsManager();

  void lockForRead(TME *me);
  void lockForWrite(TME *me);
  void unlock(TME *me);

private:
  QMap<TME*, QReadWriteLock*> map_rwl_;
};

}

#endif // ELEMENTSMANAGER_H
