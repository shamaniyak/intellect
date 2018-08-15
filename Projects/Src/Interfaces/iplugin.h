#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QtPlugin>

class IPlugin
{
public:
  virtual ~IPlugin() {}

  virtual int getVersion() = 0;
};

#define IPlugin_iid "ru.shamaniyak.Intellect.IPlugin/1.0"
Q_DECLARE_INTERFACE(IPlugin, IPlugin_iid)

#endif // IPLUGIN_H
