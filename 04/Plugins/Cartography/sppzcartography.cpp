#include "sppzcartography.h"

#include <ICartographyIntf.h>

#include <MapView.h>
#include <QLibrary>
#include <QPixmap>
#include <QtWin>
#include <QtDebug>

// Получение указателя на объект, реализующий логику работы
// с картой на основе функций картографии СППЗ
extern "C" Q_DECL_EXPORT IMap* getMap()
{
  return new SppzCartography();
}
//--------------------------------------------------------------------------------------------

SppzCartography::SppzCartography(QObject *parent) : QObject(parent)
{
  this->setObjectName("SppzCartography");
}

SppzCartography::~SppzCartography()
{
  if(cart_)
    cart_->release();
}

void SppzCartography::initLibrary()
{
  if(cart_)
    return;

  QLibrary *lib = new QLibrary("d:\\Work\\sppz\\Exe\\cartography", this);
  if(lib->load())
  {
    typedef ICartographyIntf * (*FunCreateCartography)();
    FunCreateCartography fun = (FunCreateCartography)lib->resolve("_CreateCartography");
    if(fun)
      cart_ = dynamic_cast<ICartographyIntf*>( fun() );
  }
  else
  {
    qDebug() << lib->errorString();
  }

}

void SppzCartography::BL_XmYm(double B, double L, double &Xm, double &Ym)
{
}

void SppzCartography::XmYm_BL(double Xm, double Ym, double &B, double &L)
{
}

void SppzCartography::XY_XmYm(double X, double Y, double &Xm, double &Ym)
{
}

void SppzCartography::XmYm_XY(double Xm, double Ym, double &X, double &Y)
{
}

double SppzCartography::getHeight_XmYm(double Xm, double Ym)
{
}

bool SppzCartography::getMagneticAngle_BL(double B, double L, double *Ma)
{
}

bool SppzCartography::isMtrExist()
{
}

bool SppzCartography::setMap(QString mapName)
{
}

QString SppzCartography::getMapFileName()
{
}

void SppzCartography::setMapMtr(QString mtrName)
{
}

void SppzCartography::openWorld()
{
  initLibrary();

  if(cart_)
    cart_->CreateMap();

}

void SppzCartography::setMapBright(long value)
{
}

long SppzCartography::getMapBright()
{
}

void SppzCartography::setMapContrast(long value)
{
}

long SppzCartography::getMapContrast()
{
}

void SppzCartography::setMapContour(bool value)
{
}

bool SppzCartography::getMapContour()
{
}

void SppzCartography::setViewScale(long &centerX, long &centerY, float value)
{
}

int SppzCartography::getViewScale()
{
}

int SppzCartography::getMapScale()
{
}

void SppzCartography::setMapCenter(double CenterB, double CenterL)
{
}

void SppzCartography::calcCurrentCentreRegion(double &B, double &L)
{
}

void SppzCartography::readSettings(bool restoreSettings)
{
}

void SppzCartography::writeSettings(bool restoreSettings)
{
}

void SppzCartography::writePreviousSettings()
{
}

void SppzCartography::selectLayer(int index, bool selected, bool noCheck)
{
}

void SppzCartography::selectObject(long inCode, bool selected, bool noCheck)
{
}

bool SppzCartography::isLayerSelected(int index)
{
}

bool SppzCartography::isObjectSelected(long inCode)
{
}

QString SppzCartography::getMapLayerName(int index)
{
}

QString SppzCartography::getMapObjectName(int index)
{
}

QString SppzCartography::getMapVisibleLayerName(int index)
{
}

QString SppzCartography::getMapVisibleObjectName(int index)
{
}

int SppzCartography::getLayersCount()
{
}

int SppzCartography::getVisibleLayersCount()
{
}

int SppzCartography::getVisibleObjectsCount()
{
}

void SppzCartography::setViewSelect()
{
}

int SppzCartography::getObjectsCount()
{
}

int SppzCartography::getObjectsCountInLayer(int layerIndex)
{
}

QString SppzCartography::getLayerObjectName(int layerIndex, int objectIndex)
{
}

long SppzCartography::getLayerObjectCode(int layerIndex, int objectIndex)
{
}

int SppzCartography::getLayerIndex(long inCode)
{
}

QList<long> SppzCartography::getLayerObjectsCodes(int layerIndex)
{
}

void SppzCartography::getMapImageSize(long &width, long &height)
{
  width = 0; height = 0;
  if(cart_)
  {
    cart_->GetAreaSize(width, height);
  }
}

long SppzCartography::getImageMap(int left, int top, int width, int height, QImage &_image)
{
  HBITMAP hbmp = 0;
  if(cart_)
  {
    hbmp = (HBITMAP)cart_->GetMapBitmap();
    QPixmap pxmap = QtWin::fromHBITMAP(hbmp/*, QtWin::HBitmapAlpha*/);
    _image = pxmap.toImage();
    return(true);
  }
  return(false);
}

QWidget *SppzCartography::createMapView()
{
  auto w = new myDMapView();

  w->setMapLogic(this);
  w->openWorld();

  return w;
}
