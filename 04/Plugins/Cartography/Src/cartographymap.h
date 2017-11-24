#ifndef CARTOGRAPHYMAP_H
#define CARTOGRAPHYMAP_H

#include <QObject>

class IMap;

class CartographyMap : public QObject
{
  Q_OBJECT
public:
  explicit CartographyMap(QObject *parent = 0);
  ~CartographyMap();

  IMap *map() const;
  void setMap(IMap *map);

signals:
  changed();

public slots:
  void setFileMap(const QString &name);

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
};

#endif // CARTOGRAPHYMAP_H
