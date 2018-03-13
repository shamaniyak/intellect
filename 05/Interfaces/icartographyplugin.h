#ifndef ICARTOGRAPHYLUGIN_H
#define ICARTOGRAPHYLUGIN_H

#include <QtPlugin>

class ICartographyPlugin
{
public:
  virtual ~ICartographyPlugin() {}

  virtual int getVersion() = 0;
};

#define ICartographyPlugin_iid "ru.shamaniyak.Intellect.ICartographyPlugin/1.0"
Q_DECLARE_INTERFACE(ICartographyPlugin, ICartographyPlugin_iid)

#endif // ICARTOGRAPHYLUGIN_H
