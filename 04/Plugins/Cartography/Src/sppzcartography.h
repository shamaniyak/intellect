#ifndef SPPZCARTOGRAPHY_H
#define SPPZCARTOGRAPHY_H

#include <imap.h>

class QImage;
class ICartographyIntf;
class myDMapView;

class SppzCartography : public QObject, public IMap
{
  Q_OBJECT

public:
  explicit SppzCartography(QObject *parent=0);
  virtual ~SppzCartography();

  virtual void release() { delete this; }

  void initLibrary();

  // IMap interface
public slots:

  virtual void BL_XmYm(double B, double L, double &Xm, double &Ym) override;
  virtual void XmYm_BL(double Xm, double Ym, double &B, double &L) override;
  virtual void XY_XmYm(double X, double Y, double &Xm, double &Ym) override;
  virtual void XmYm_XY(double Xm, double Ym, double &X, double &Y) override;
  virtual void BL_XY(double B, double L, double &X, double &Y) override;
  virtual void XY_BL(double X, double Y, double &B, double &L) override;
  virtual double getHeight_XmYm(double Xm, double Ym) override;
  virtual double getHeight_XY(double x, double y) override;
  virtual void getMapCentre(double &B, double &L) override;
  virtual bool getMagneticAngle_BL(double B, double L, double *Ma) override;
  virtual bool isMtrExist() override;
  virtual bool setMap(QString mapName) override;
  virtual QString getMapFileName() override;
  virtual void setMapMtr(QString mtrName) override;
  virtual void openWorld() override;
  virtual void setMapBright(long value) override;
  virtual long getMapBright() override;
  virtual void setMapContrast(long value) override;
  virtual long getMapContrast() override;
  virtual void setMapContour(bool value) override;
  virtual bool getMapContour() override;
  virtual void setViewScale(long &centerX, long &centerY, float value) override;
  virtual int getViewScale() override;
  virtual int getMapScale() override;
  virtual void setMapCenter(double CenterB, double CenterL) override;
  virtual void calcCurrentCentreRegion(double &B, double &L) override;
  virtual void readSettings(bool restoreSettings) override;
  virtual void writeSettings(bool restoreSettings) override;
  virtual void writePreviousSettings() override;
  virtual void selectLayer(int index, bool selected, bool noCheck) override;
  virtual void selectObject(long inCode, bool selected, bool noCheck) override;
  virtual bool isLayerSelected(int index) override;
  virtual bool isObjectSelected(long inCode) override;
  virtual QString getMapLayerName(int index) override;
  virtual QString getMapObjectName(int index) override;
  virtual QString getMapVisibleLayerName(int index) override;
  virtual QString getMapVisibleObjectName(int index) override;
  virtual int getLayersCount() override;
  virtual int getVisibleLayersCount() override;
  virtual int getVisibleObjectsCount() override;
  virtual void setViewSelect() override;
  virtual int getObjectsCount() override;
  virtual int getObjectsCountInLayer(int layerIndex) override;
  virtual QString getLayerObjectName(int layerIndex, int objectIndex) override;
  virtual long getLayerObjectCode(int layerIndex, int objectIndex) override;
  virtual int getLayerIndex(long inCode) override;
  virtual QList<long> getLayerObjectsCodes(int layerIndex) override;
  virtual void getMapImageSize(long &width, long &height) override;
  virtual long getImageMap(int left, int top, int width, int height, QImage &_image) override;

  QWidget * createMapView();

private:
  ICartographyIntf *cart_ = nullptr;
  myDMapView *mapView_ = nullptr;
};

#endif // SPPZCARTOGRAPHY_H
