#ifndef ISPPZCARTOGRAPHYLUGIN_H
#define ISPPZCARTOGRAPHYLUGIN_H

#include <QtPlugin>

class ISppzCartographyPlugin
{
public:
  virtual ~ISppzCartographyPlugin() {}

  virtual int getVersion() = 0;
};

#define ISppzCartographyPlugin_iid "ru.shamaniyak.Intellect.ISppzCartographyPlugin/1.0"
Q_DECLARE_INTERFACE(ISppzCartographyPlugin, ISppzCartographyPlugin_iid)

#endif // ISPPZCARTOGRAPHYLUGIN_H
