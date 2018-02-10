#ifndef MAPGRAPHICSPIXMAPITEM_H
#define MAPGRAPHICSPIXMAPITEM_H

#include <QGraphicsPixmapItem>

const int MAXSCALE_40M = 40000000;
const int MAX_ZOOM = 32;
const double MIN_ZOOM = 0.03125;

class IMap;

class MapGraphicsPixmapItem : public QGraphicsPixmapItem
{
public:
  explicit MapGraphicsPixmapItem(QGraphicsItem *parent = nullptr);

  void setMap(IMap *map);

  void resize(int w, int h);

  QPointF imageLT() const;
  QPointF center() const;
  void setCenter(const QPointF &pos);
  // �������� ������� � ������� ���� � ����������� �� delta
  void zoomInOut(const QPointF &pos, int delta);

protected:

  void setImageLT(const QPointF &pos);
  // ��������� ������������ ��������� �������� ������ ����
  void calcMaxImageLT();

  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

  virtual void wheelEvent(QGraphicsSceneWheelEvent *event) override;

  // ��� ��������� ���������
  bool sceneEvent(QEvent *event) override;
  bool touchEvent(QEvent *e);

private:
  IMap *map_ = nullptr;
  QImage image_;
  bool canMove_ = false;
  QPointF imageLT_;
  QPointF maxImageLT_;
  int imageW_ = 250;
  int imageH_ = 250;
  bool drawed_ = false;

  //�������� ������� ����������� ����������
  double GetMapZoom();
  // ���������� ������� �����������
  void SetViewScale(double b, double l, float value);
  // ���������� ������������� ���������� ������ ������������ �����
  void setMapCenter(double B, double L);
  //����������� ����������� �� ������� ����� � ������� ����
  //���� ���������� �������, �� ������������ �������� ������ �����
  void ZoomMapIn(double B = 0, double L = 0);
  //��������� ����������� �� ������� ����� � ������� ����
  //���� ���������� �������, �� ������������ �������� ������ �����
  void ZoomMapOut(double B = 0, double L = 0);

};

#endif // MAPGRAPHICSPIXMAPITEM_H
