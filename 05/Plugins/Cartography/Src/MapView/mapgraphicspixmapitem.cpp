#include "mapgraphicspixmapitem.h"

#include <imap.h>

#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>

//  MapGraphicsPixmapItem

MapGraphicsPixmapItem::MapGraphicsPixmapItem(QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
  //setFlags(QGraphicsItem::ItemIsMovable);
}

void MapGraphicsPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
  if(canMove_)
  {
    auto dp = e->scenePos() - e->lastScenePos();
    setImageLT(imageLT_ -= dp);
  }
}

void MapGraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
  canMove_ = true;

  calcMaxImageLT();
}

void MapGraphicsPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
  canMove_ = false;
}

void MapGraphicsPixmapItem::setMap(IMap *map)
{
  map_ = map;
  drawed_ = false;
}

void MapGraphicsPixmapItem::resize(int w, int h)
{
  imageW_ = w; imageH_ = h;
  drawed_ = false;
}

void MapGraphicsPixmapItem::setImageLT(const QPointF &pos)
{
  imageLT_ = pos;
  drawed_ = false;

  if(imageLT_.x() < 0) imageLT_.setX(0);
  if(imageLT_.y() < 0) imageLT_.setY(0);
  if(imageLT_.x() > maxImageLT_.x()) imageLT_.setX(maxImageLT_.x());
  if(imageLT_.y() > maxImageLT_.y()) imageLT_.setY(maxImageLT_.y());

  this->update();
}

void MapGraphicsPixmapItem::calcMaxImageLT()
{
  if(map_)
  {
    long int mapW, mapH;
    map_->getMapImageSize(mapW, mapH);

    maxImageLT_ = QPointF(mapW - imageW_, mapH - imageH_);
    if(maxImageLT_.x() < 0) maxImageLT_.setX(0);
    if(maxImageLT_.y() < 0) maxImageLT_.setY(0);
  }
}

void MapGraphicsPixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  if(map_ && !drawed_)
  {
    drawed_ = true;
    if(map_->getImageMap(imageLT_.x(), imageLT_.y(), imageW_, imageH_, image_))
    {
      //bool saved = image_.save("c:\\image.png");

      setPixmap(QPixmap::fromImage(image_));
    }
  }

  QGraphicsPixmapItem::paint(painter, option, widget);
}


void MapGraphicsPixmapItem::wheelEvent(QGraphicsSceneWheelEvent *e)
{
  //if(e->modifiers().testFlag(Qt::ControlModifier))
  {
    int delta = e->delta();
    auto p = imageLT_ + e->scenePos();
    zoomInOut(p, delta);
  }

  //QGraphicsView::wheelEvent(e);
}

bool MapGraphicsPixmapItem::sceneEvent(QEvent *event)
{
  switch( event->type() )
  {
    case QEvent::TouchBegin:
    case QEvent::TouchEnd:
    case QEvent::TouchUpdate:
    case QEvent::TouchCancel:
      return touchEvent(event);
  }

  QGraphicsPixmapItem::sceneEvent(event);
}

bool MapGraphicsPixmapItem::touchEvent(QEvent *e)
{
  switch( e->type() )
  {
    case QEvent::TouchBegin:
    case QEvent::TouchEnd:
    case QEvent::TouchCancel:
      break;
    case QEvent::TouchUpdate:
    {
      QTouchEvent *te = dynamic_cast<QTouchEvent*>(e);
      if(te)
      {
        auto points = te->touchPoints();
        auto cnt = points.size();
        if(cnt == 2)
        {
          QRectF lastRect(points[0].lastScenePos(), points[1].lastScenePos());
          QRectF rect(points[0].scenePos(), points[1].scenePos());
          auto lastWidth = lastRect.width();
          auto width = rect.width();
          auto lastHeight = lastRect.height();
          auto height = rect.height();
          auto p = imageLT_ + rect.center();
          int delta = 0;
          if(lastWidth < width || lastHeight < height)
            delta = 1;
          if(lastWidth > width || lastHeight > height)
            delta = -1;

          zoomInOut(p, delta);
        }
      }
      return true;
    }
  }

  return false;
}

QPointF MapGraphicsPixmapItem::imageLT() const
{
  return imageLT_;
}

QPointF MapGraphicsPixmapItem::center() const
{
  return imageLT_ + QPointF(imageW_/2, imageH_/2);
}

void MapGraphicsPixmapItem::setCenter(const QPointF &pos)
{
  setImageLT(pos - QPointF(imageW_/2, imageH_/2));
}

double MapGraphicsPixmapItem::GetMapZoom()
{
  double result = 0;
  if(map_)
  {
    double ms = map_->getMapScale();
    double vs = map_->getViewScale();
    result = ms / vs;
  }
  return(result);
}

void MapGraphicsPixmapItem::ZoomMapIn(double B, double L)
{
  if (GetMapZoom() < MAX_ZOOM)
  {
    auto s = map_->getViewScale();
    SetViewScale(B, L, s / 1.5);
  }
}

void MapGraphicsPixmapItem::ZoomMapOut(double B, double L)
{
  if (GetMapZoom() > MIN_ZOOM)
  {
    auto s = map_->getViewScale();
    float newView = s*1.5;
    if (newView > MAXSCALE_40M) newView = MAXSCALE_40M;
    SetViewScale(B, L, newView);
  }
}

void MapGraphicsPixmapItem::SetViewScale(double b, double l, float value)
{
  if(map_)
  {
    double X, Y;
    map_->BL_XY(b, l, X, Y);
    map_->setViewScale((long&)X, (long&)Y, value);
    calcMaxImageLT();
  }
}

void MapGraphicsPixmapItem::setMapCenter(double B, double L)
{
  if(map_)
  {
    map_->setMapCenter(B, L);

    double centreX, centreY;
    map_->BL_XY(B, L, centreX, centreY);

    int left = (int)centreX - imageW_ / 2;
    int top = (int)centreY - imageH_ / 2;

    setImageLT( QPointF(left, top) );
  }
}

void MapGraphicsPixmapItem::zoomInOut(const QPointF &p, int delta)
{
  double b = 0, l = 0;
  if(map_)
    map_->XY_BL(p.x(), p.y(), b, l);

  if(delta > 0)
  {
    ZoomMapIn(b, l);
  }
  else if(delta < 0)
  {
    ZoomMapOut(b, l);
  }

  if(map_)
  {
    double x, y;
    map_->BL_XY(b, l, x, y);
    QPointF dp = p - QPointF(x, y);
    if(!dp.isNull())
    {
      setImageLT( imageLT_ - dp );
    }
  }

  drawed_ = false;
}
