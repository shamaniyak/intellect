#ifndef IMULTIMEDIA_H
#define IMULTIMEDIA_H

#include <QtPlugin>

class IMultimediaPlugin
{
public:
  virtual ~IMultimediaPlugin() {}

  virtual int getVersion() = 0;
};

#define IMultimediaPlugin_iid "ru.shamaniyak.Intellect.IMultimediaPlugin/1.0"
Q_DECLARE_INTERFACE(IMultimediaPlugin, IMultimediaPlugin_iid)

#endif // IMULTIMEDIA_H
