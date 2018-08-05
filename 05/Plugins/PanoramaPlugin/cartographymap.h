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

  // Установить уровень яркости карты (-16  16)
  void setMapBright(long int value);
  // Получить уровень яркости
  long int getMapBright();
  // Установить уровень контраста изображения
  void setMapContrast(long int value);
  // Получить текущий уровень контраста
  long int getMapContrast();
  // Установить контурность изображения (true - значит контурная карта)
  void setMapContour(bool value);
  // Получить признак контурной карты (true - значит контурная)
  bool getMapContour();
  // Установить масштаб отображения
  void setViewScale(float value);
  // Получить масштаб отображения
  int getViewScale();
  // Получить масштаб карты
  int getMapScale();
  //Получить текущий коэффициент увеличения
  double getMapZoom();
  //Увеличивает изображение на текущей карте в два раза
  void zoomIn(double B = 0, double L = 0);
  //Уменьшает изображение на текущей карте в два раза
  void zoomOut(double B = 0, double L = 0);
  // Установить геодезические координаты центра отображаемой карты
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
