#include "cartography.h"
#include "MapView/MapView.h"
#include "cartographymap.h"

#include <QLibrary>
#include <QtDebug>


Cartography::Cartography(QObject *parent) : QObject(parent),
  parentWidget_(new QWidget())
{
  setObjectName("Cartography");
}

Cartography::~Cartography()
{
  delete parentWidget_;
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

  //if(!parent)
  //  parent = parentWidget_;

  if(cmap) {
    myDMapView *mv = new myDMapView(parent);
    mv->setMap(cmap);
    return mv;
  }

  return nullptr;
}
