#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsProxyWidget>

class GraphicsView : public QGraphicsView
{
  Q_OBJECT
public:
  explicit GraphicsView(QWidget *parent = 0);

  void createLayout();

signals:

public slots:
  QObject *addWidget(QWidget *wgt);

private:
  QGraphicsScene scene_;
};

class ProxyWidget : public QGraphicsProxyWidget
{
  Q_OBJECT
  Q_PROPERTY(bool editable READ getEditable WRITE setEditable)
public:
  explicit ProxyWidget(QGraphicsItem *parent = 0);

  bool getEditable() const;
  void setEditable(bool editable);

protected:
  bool sceneEvent(QEvent *event) override;

  void mousePressEvent(QGraphicsSceneMouseEvent *event);

  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
  bool editable_ = true;
};


#endif // GRAPHICSVIEW_H
