#ifndef CARTOGRAPHYMAP_H
#define CARTOGRAPHYMAP_H

#include <QObject>
#include <QImage>

class IMap;

class CartographyMap : public QObject
{
  Q_OBJECT
	Q_PROPERTY(QImage image READ image NOTIFY imageChanged)
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

	QImage image();

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
	QPointF imageLT_;
	QPointF maxImageLT_;
	int imageW_ = 250;
	int imageH_ = 250;
};

#endif // CARTOGRAPHYMAP_H
