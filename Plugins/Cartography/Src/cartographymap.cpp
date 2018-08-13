#include "cartographymap.h"
#include <imap.h>

#include <QtDebug>
#include <QLibrary>

static const int MAXSCALE_40M = 40000000;
static const int MAX_ZOOM = 32;
static const double MIN_ZOOM = 0.03125;

CartographyMap::CartographyMap(QObject *parent) : QObject(parent)
{
  init();
}

CartographyMap::~CartographyMap()
{
  if(map_)
    map_->release();
}

void CartographyMap::init()
{
  if(!map_)
  {
    map_ = createMapIntegration();
  }
}

IMap *CartographyMap::createMapIntegration()
{
  IMap *map = nullptr;

  // Чтобы загрузить, нужно добавить в переменную окружения PATH путь к DLL, реализующий интерфейс IMap
  QLibrary lib("mapintegration");
  if(lib.load())
  {

    typedef IMap* (*FgetMap)();
    FgetMap getMap = (FgetMap)lib.resolve("getMap");
    if(getMap)
      map = getMap();
  }
  else
  {
    qDebug() << lib.errorString();
  }

  return map;
}

IMap *CartographyMap::map() const
{
  return map_;
}

void CartographyMap::setMap(IMap *map)
{
  map_ = map;
  emit changed();
}

void CartographyMap::setFileMap(const QString &name)
{
  if(map_)
  {
    map_->setMap(name);
    emit changed();
	}
}

QImage CartographyMap::image()
{
	static QImage image_;
	if(map_)
	{
		if(map_->getImageMap(imageLT_.x(), imageLT_.y(), imageW_, imageH_, image_))
		{
			//bool saved = image_.save("c:\\image.png");
			return image_;
		}
	}
	return QImage();
}

void CartographyMap::setMapBright(long value)
{
  if(map_)
  {
    map_->setMapBright(value);
    emit changed();
  }
}

long CartographyMap::getMapBright()
{
  if(map_)
  {
    return map_->getMapBright();
  }
  return 0;
}

void CartographyMap::setMapContrast(long value)
{
  if(map_)
  {
    map_->setMapContrast(value);
    emit changed();
  }
}

long CartographyMap::getMapContrast()
{
  if(map_)
  {
    return map_->getMapContrast();
  }
  return 0;
}

void CartographyMap::setMapContour(bool value)
{
  if(map_)
  {
    map_->setMapContour(value);
    emit changed();
  }
}

bool CartographyMap::getMapContour()
{
  if(map_)
  {
    return map_->getMapContour();
  }
  return false;
}

void CartographyMap::setViewScale(float value)
{
  if(map_)
  {
    long cx = 100, cy = 100;
    map_->setViewScale(cx, cy, value);
    emit changed();
  }
}

int CartographyMap::getViewScale()
{
  if(map_)
  {
    return map_->getViewScale();
  }
  return 0;
}

int CartographyMap::getMapScale()
{
  if(map_)
  {
    return map_->getMapScale();
  }
  return 0;
}

double CartographyMap::getMapZoom()
{
  if(map_)
  {
    double ms = map_->getMapScale();
    double vs = map_->getViewScale();
    return(ms / vs);
  }
  return 0;
}

void CartographyMap::zoomIn(double B, double L)
{
  if(map_)
  {
    if (getMapZoom() < MAX_ZOOM)
    {
      setViewScale(getViewScale()/2);
      if (B != 0 || L != 0)
      {
        setMapCenter(B, L);
      }
      emit changed();
    }
  }
}

void CartographyMap::zoomOut(double B, double L)
{
  if(map_)
  {
    if (getMapZoom() > MIN_ZOOM)
    {
      uint newView = (getViewScale())*2;
      if (newView > MAXSCALE_40M)
        newView = MAXSCALE_40M;
      setViewScale(newView);
      if (B != 0 || L != 0)
      {
        setMapCenter(B, L);
      }
      emit changed();
    }
  }
}

void CartographyMap::setMapCenter(double centerB, double centerL)
{
  if(map_)
  {
    map_->setMapCenter(centerB, centerL);
    emit changed();
  }
}
