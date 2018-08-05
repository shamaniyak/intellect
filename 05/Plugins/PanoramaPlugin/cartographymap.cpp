#include "cartographymap.h"
#include <imap.h>

#include <QtDebug>
#include <QLibrary>
#include <QPainter>

static const int MAXSCALE_40M = 40000000;
static const int MAX_ZOOM = 32;
static const double MIN_ZOOM = 0.03125;

CartographyMap::CartographyMap(QObject *parent) : QQuickPaintedItem()
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
	setAntialiasing(true);
	setOpaquePainting(true);
  if(!map_)
  {
    map_ = createMapIntegration();
  }
}

IMap *CartographyMap::createMapIntegration()
{
  IMap *map = nullptr;

  // ����� ���������, ����� �������� � ���������� ��������� PATH ���� � DLL, ����������� ��������� IMap
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

void CartographyMap::paint(QPainter *painter)
{
	refreshImage();
	painter->drawImage(0, 0, image_);
}

void CartographyMap::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
	//Q_UNUSED(oldGeometry)
	size_ = newGeometry;
	QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
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
		long w, h;
		map_->getMapImageSize(w, h);
		setSize(QSizeF(w, h));
    emit changed();
	}
}

void CartographyMap::refreshImage()
{
	if(map_)
	{

		if(map_->getImageMap(imageLT_.x(), imageLT_.y(), size_.width(), size_.height(), image_))
		{
			//bool saved = image_.save("c:\\image.png");
		}
	}
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

QPointF CartographyMap::BLtoXY(double B, double L)
{
	if(map_) {
		double x, y;
		map_->BL_XY(B, L, x, y);
		return QPointF(x, y);
	}
	return QPointF();
}

QPointF CartographyMap::XYtoBL(double X, double Y)
{
	if(map_) {
		double b, l;
		map_->XY_BL(X, Y, b, l);
		return QPointF(b, l);
	}
	return QPointF();
}
