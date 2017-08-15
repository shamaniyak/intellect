#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include "myplugin_global.h"

#include <Interfaces/iplugin.h>

class MYPLUGINSHARED_EXPORT MyPlugin : public QObject, public IPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID IPlugin_iid)
  Q_INTERFACES(IPlugin)

public:
  MyPlugin();

  // IPlugin interface
public:
  virtual int getVersion() override
  {
    return 1;
  }

public slots:
  int getVer();
};

#endif // MYPLUGIN_H
