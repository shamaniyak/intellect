#include "cartography.h"
#include "MapView/MapView.h"
#include "cartographymap.h"

#include <QLibrary>
#include <QtDebug>

Cartography::Cartography(QObject *parent) : QObject(parent)
{
  setObjectName("Cartography");
}

QObject *Cartography::createMap()
{
  if(!map_) {
    CartographyMap *cmap = new CartographyMap(this);
    cmap->setObjectName("Map");
    map_ = cmap;
  }
  return map_;
}

QWidget *Cartography::createMapView(QObject *map, QWidget *parent)
{
  if(!w_)
  {
    auto cmap = qobject_cast<CartographyMap*>(map);

    if(cmap) {
      myDMapView *mv = new myDMapView(parent);
      mv->setMap(cmap);
      w_ = mv;
    }
  }

  return w_;
}
