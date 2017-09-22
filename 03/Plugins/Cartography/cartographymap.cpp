#include "cartographymap.h"
#include <imap.h>

#include <QtDebug>
#include <QLibrary>

CartographyMap::CartographyMap(QObject *parent) : QObject(parent)
{

}

void CartographyMap::init()
{
  if(!map_)
  {
    //QString appDirPath = QApplication::applicationDirPath();
    //QString worldPath = appDirPath + "/../../Maps/World/world.map";

    QLibrary lib("mapintegration");
    if(lib.load())
    {
      typedef IMap* (*FgetMap)();
      FgetMap getMap = (FgetMap)lib.resolve("getMap");
      if(getMap)
        map_ = getMap();
    }
    else
    {
      qDebug() << lib.errorString();
    }
  }
}

IMap *CartographyMap::map() const
{
  return map_;
}

void CartographyMap::setMap(IMap *map)
{
  map_ = map;
}

void CartographyMap::setFileMap()
{

}
