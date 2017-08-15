#ifndef ISPPZCLASSIFIERPLUGIN_H
#define ISPPZCLASSIFIERPLUGIN_H

#include <QtPlugin>

class ISppzClassifierPlugin
{
public:
  virtual ~ISppzClassifierPlugin() {}

  virtual int getVersion() = 0;
};

#define ISppzClassifierPlugin_iid "ru.shamaniyak.Intellect.ISppzClassifierPlugin/1.0"
Q_DECLARE_INTERFACE(ISppzClassifierPlugin, ISppzClassifierPlugin_iid)

#endif // ISPPZCLASSIFIERPLUGIN_H
