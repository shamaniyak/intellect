#include "cartography.h"
#include "sppzcartography.h"
#include "MapView/MapView.h"

#include <QtDebug>

Cartography::Cartography(QObject *parent) : QObject(parent)
{
  setObjectName("Cartography");
}

QObject *Cartography::createMap()
{
  //auto map = new SppzCartography(this);
  auto map = createMapIntegration();
  return map;
}

QWidget *Cartography::createMapView(QObject *map, QWidget *parent)
{
  auto w = new myDMapView(parent);
  w->setMapLogic(qobject_cast<IMap*>(map));
  w->openWorld();

  return w;
}

QObject *Cartography::createMapIntegration()
{

}
