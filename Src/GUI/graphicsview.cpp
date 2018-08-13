#include "graphicsview.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>

static QGraphicsItem::GraphicsItemFlags gFlagsMS =
    QGraphicsItem::ItemIsMovable |
    QGraphicsItem::ItemIsSelectable;




GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
  setScene(&scene_);

  scene_.setItemIndexMethod(QGraphicsScene::NoIndex);
  //scene_.setSceneRect(0, 0, 640, 480);

  QGraphicsLinearLayout *layout;
}

void GraphicsView::createLayout()
{
  //auto l = new QGraphicsLinearLayout();

}

QObject *GraphicsView::addWidget(QWidget *wgt)
{
  if(!wgt)
    return nullptr;
  wgt->setParent(0);

  auto proxyWidget = new ProxyWidget();
  proxyWidget->setWidget(wgt);
  scene_.addItem(proxyWidget);

  return( proxyWidget );
}

//  ProxyWidget

ProxyWidget::ProxyWidget(QGraphicsItem *parent) : QGraphicsProxyWidget(parent)
{
  setFlags(gFlagsMS);
}

bool ProxyWidget::getEditable() const
{
  return editable_;
}

void ProxyWidget::setEditable(bool editable)
{
  editable_ = editable;
}

bool ProxyWidget::sceneEvent(QEvent *event)
{
  return QGraphicsItem::sceneEvent(event);
}

void ProxyWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  //QPointF pos = event->pos();
  //      /auto alienWidget = widget()->childAt(pos.toPoint());
  if (!editable_)
  {
    QGraphicsProxyWidget::mousePressEvent(event);
  }
  else
  {
    QGraphicsItem::mousePressEvent(event);
  }
}

void ProxyWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  if (!editable_)
    QGraphicsProxyWidget::mouseReleaseEvent(event);
  else
    QGraphicsItem::mouseReleaseEvent(event);
  //grabbedByWidget=false;
}

void ProxyWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  if (!editable_)
    return;
  QGraphicsItem::mouseMoveEvent(event);
}
