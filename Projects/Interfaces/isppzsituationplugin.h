#ifndef ISPPZSITUATIONPLUGIN_H
#define ISPPZSITUATIONPLUGIN_H

#include <QtPlugin>

class ISppzSituationPlugin
{
public:
  virtual ~ISppzSituationPlugin() {}

  virtual int getVersion() = 0;
};

#define ISppzSituationPlugin_iid "ru.shamaniyak.Intellect.ISppzSituationPlugin/1.0"
Q_DECLARE_INTERFACE(ISppzSituationPlugin, ISppzSituationPlugin_iid)

#endif // ISPPZSITUATIONPLUGIN_H
