#ifndef IWINDOWMANAGER_H
#define IWINDOWMANAGER_H

#include <QtPlugin>

class IWindowManager
{
public:
  virtual ~IWindowManager() {}

  virtual int getVersion() = 0;
};

#define IWindowManager_iid "ru.shamaniyak.Intellect.IWindowManager/1.0"
Q_DECLARE_INTERFACE(IWindowManager, IWindowManager_iid)

#endif // IWINDOWMANAGER_H
