#include "cartography.h"
#include "MapView/MapView.h"
#include "cartographymap.h"

#include <QLibrary>
#include <QtDebug>


Cartography::Cartography(QObject *parent) : QObject(parent)
{
  setObjectName("Cartography");
}

Cartography::~Cartography()
{

}

QObject *Cartography::createMap()
{
  //if(!map_) {
    CartographyMap *cmap = new CartographyMap();
    cmap->setObjectName("Map");
    map_ = cmap;
    return cmap;
  //}
  //return map_;
}

QWidget *Cartography::createMapView(QObject *map, QWidget *parent)
{
  auto cmap = qobject_cast<CartographyMap*>(map);

  if(cmap) {
    MapView *mv = new MapView(parent);
    mv->setMap(cmap);
    return mv;
  }

  return nullptr;
}
