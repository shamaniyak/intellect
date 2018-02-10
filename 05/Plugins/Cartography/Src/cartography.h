#ifndef CARTOGRAPHY_H
#define CARTOGRAPHY_H

#include "cartography_global.h"
#include <icartographyplugin.h>

class CARTOGRAPHYSHARED_EXPORT Cartography : public QObject, public ICartographyPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ICartographyPlugin_iid)
  Q_INTERFACES(ICartographyPlugin)

public:
  explicit Cartography(QObject *parent = 0);
  ~Cartography();

signals:

public slots:
  QObject *createMap();
  QWidget * createMapView(QObject *map, QWidget *parent = 0);

  // ICartographyPlugin interface
public:
  virtual int getVersion() override
  {
    return 1;
  }

protected:

private:
  QObject *map_ = nullptr;
  QWidget *parentWidget_ = nullptr;
};

#endif // CARTOGRAPHY_H
