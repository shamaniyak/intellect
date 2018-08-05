#ifndef CARTOGRAPHYMAP_H
#define CARTOGRAPHYMAP_H

#include <QObject>
#include <QImage>
#include <QQuickPaintedItem>

class IMap;

class CartographyMap : public QQuickPaintedItem
{
	Q_OBJECT
public:
  explicit CartographyMap(QObject *parent = 0);
  ~CartographyMap();

  IMap *map() const;
  void setMap(IMap *map);

signals:
	void changed();
	void imageChanged();

public slots:
  void setFileMap(const QString &name);

	void refreshImage();

  // ���������� ������� ������� ����� (-16  16)
  void setMapBright(long int value);
  // �������� ������� �������
  long int getMapBright();
  // ���������� ������� ��������� �����������
  void setMapContrast(long int value);
  // �������� ������� ������� ���������
  long int getMapContrast();
  // ���������� ����������� ����������� (true - ������ ��������� �����)
  void setMapContour(bool value);
  // �������� ������� ��������� ����� (true - ������ ���������)
  bool getMapContour();
  // ���������� ������� �����������
  void setViewScale(float value);
  // �������� ������� �����������
  int getViewScale();
  // �������� ������� �����
  int getMapScale();
  //�������� ������� ����������� ����������
  double getMapZoom();
  //����������� ����������� �� ������� ����� � ��� ����
  void zoomIn(double B = 0, double L = 0);
  //��������� ����������� �� ������� ����� � ��� ����
  void zoomOut(double B = 0, double L = 0);
  // ���������� ������������� ���������� ������ ������������ �����
  void setMapCenter(double centerB, double centerL);

protected:
  void init();
  IMap *createMapIntegration();

private:
  IMap *map_ = nullptr;
	QImage image_;
	QPointF imageLT_;
	QPointF maxImageLT_;
	QRectF size_;

	// QQuickPaintedItem interface
public:
	void paint(QPainter *painter);

	// QQuickItem interface
protected:
	void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);
};

#endif // CARTOGRAPHYMAP_H
