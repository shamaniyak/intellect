#include "graphicsview.h"

#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>

static QGraphicsItem::GraphicsItemFlags gFlagsMS =
    QGraphicsItem::ItemIsMovable |
    QGraphicsItem::ItemIsSelectable;

class ProxyWidget : public QGraphicsProxyWidget
{
public:
  explicit ProxyWidget(QGraphicsItem *parent = 0) : QGraphicsProxyWidget(parent)
  {
    setFlags(gFlagsMS);
  }

protected:
  bool sceneEvent(QEvent *event) Q_DECL_OVERRIDE
  {
    return QGraphicsItem::sceneEvent(event);
  }

  void mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    //QPointF pos = event->pos();
//      /auto alienWidget = widget()->childAt(pos.toPoint());
    if (!movable_)
    {
      QGraphicsProxyWidget::mousePressEvent(event);
    }
    else
    {
      QGraphicsItem::mousePressEvent(event);
    }
  }

  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
      if (!movable_)
          QGraphicsProxyWidget::mouseReleaseEvent(event);
      else
          QGraphicsItem::mouseReleaseEvent(event);
      //grabbedByWidget=false;
  }

  void mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
      if (!movable_)
          return;
      QGraphicsItem::mouseMoveEvent(event);
  }

private:
  bool movable_ = true;
};



GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
  setScene(&scene_);

  //scene_.setSceneRect(0, 0, 640, 480);
  auto item = scene_.addRect(0, 0, 640, 480);
  item->setFlags(gFlagsMS);
}

QObject *GraphicsView::addWidget(QWidget *wgt)
{
  if(!wgt)
    return nullptr;
  wgt->setParent(0);

//  auto rect = scene_.addRect(0, 0, wgt->width(), wgt->height());
//  rect->setFlags(gFlagsMS);

  auto proxyWidget = new ProxyWidget();
  proxyWidget->setWidget(wgt);
  scene_.addItem(proxyWidget);

//  auto proxyWidget = scene_.addWidget(wgt);
//  proxyWidget->installEventFilter(this);
//  proxyWidget->setPos(0, 0);
//  proxyWidget->setFlags(gFlagsMS);

  return( proxyWidget );
}
