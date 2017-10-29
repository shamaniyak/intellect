//	Этот файл является частью проекта ПС "Конструктор".
//	Время создания: Вск Окт 7 13:24:30 2007
#include "MapView.h"

#include <algorithm>
#include <assert.h>
#include <math.h>
#include <iostream>

#include <QResizeEvent>
#include <QPixmap>
#include <QMouseEvent>
#include <QFile>
#include <QGraphicsPixmapItem>
#include <QGraphicsSimpleTextItem>
#include <QtDebug>
#include <cartographymap.h>

#include "imap.h"
#include "version.h"
#include "compare.h"


int GetMaxDistanceToDraw(int scale)
{
  double maxDistance = 4500000;
  double scaleDivider = 1;
  switch(scale)
  {
    case 1000000: scaleDivider = 1;
      break;
    case 500000: scaleDivider = 1.5;
      break;
    case 250000: scaleDivider = 2;
      break;
    case 200000: scaleDivider = 2;
      break;
    case 100000: scaleDivider = 4;
      break;
    case 50000: scaleDivider = 6;
      break;
  }
  return maxDistance/scaleDivider;
}

myDMapView :: myDMapView(QWidget *parent) : QGraphicsView(parent), map_(NULL)
{
  viewport()->setMouseTracking(true);
  viewport()->setAttribute(Qt::WA_AcceptTouchEvents);

  setScene(&scene_);

  graphicsItemMap_ = new MapGraphicsPixmapItem();
  scene_.addItem(graphicsItemMap_);
  graphicsItemMap_->setPos(QPointF(0,0));

  textItem_ = new QGraphicsSimpleTextItem(graphicsItemMap_);
  textItem_->setPos(0, 0);
  //scene_.addItem(textItem_);

  mapTaskDraw.clear();
  mapListTaskInvalidateForTask.clear();
  it_activeDrawTask = mapTaskDraw.end();

  keyboardGrabbed = false;
  isTaskPixmapFilled = false;
}
//--------------------------------------------------------------------------------------------

myDMapView :: ~myDMapView()
{
  map<QString, TBaseClassDraw* > :: iterator it = mapTaskDraw.begin();
  vector<QString> vectNameTask;
  for(; it != mapTaskDraw.end();it++)
    vectNameTask.push_back(it->first);
  for(unsigned int i = 0; i < vectNameTask.size(); i++)
    DeleteTaskDraw(vectNameTask[i]);
}
//--------------------------------------------------------------------------------------------

void myDMapView :: setMapLogic(IMap* map)
{
  map_ = map;

  graphicsItemMap_->setMap(map_);
}

void myDMapView::setMap(CartographyMap *map)
{
  if(map)
  {
    setMapLogic(map->map());
    connect(map, &CartographyMap::changed, this, &myDMapView::on_change);
  }
}

// ПЕРЕВОД КООРДИНАТ

// перевод координат из геодезических в пиксели картинки карты
void myDMapView :: BL_XY(double B, double L, double& X, double& Y)
{
  double Xm, Ym;
  BL_XmYm(B, L, Xm, Ym);
  XmYm_XY(Xm, Ym, X, Y);
}
//--------------------------------------------------------------------------------------------

// перевод координат из пикселей картинки карты в геодезические
void myDMapView :: XY_BL(double X, double Y, double& B, double& L)
{
  double Xm, Ym;
  XY_XmYm(X, Y, Xm, Ym);
  XmYm_BL(Xm, Ym, B, L);

  if( L > (M_PI - VERY_SMALL_VALUE) )
    L = L - 2*M_PI;
}
//--------------------------------------------------------------------------------------------

// перевод координат из геодезических в пиксели картинки карты
void myDMapView :: BL_XY(double B, double L, int* X, int* Y)
{
  double tempX;
  double tempY;

  BL_XY(B, L, tempX, tempY);

  *X = (int) tempX;
  *Y = (int) tempY;
}

// перевод координат из пикселей картинки карты в геодезические
void myDMapView :: XY_BL(int X, int Y, double* B, double* L)
{
  XY_BL((double) X, (double) Y, *B, *L);
}

//перевод геодезических координат  в координаты прямоугольной плоской системы
void myDMapView :: BL_XmYm(double B, double L, double& Xm, double& Ym)
{
  assert(map_ != NULL && "BL_XmYm");

  map_->BL_XmYm(B, L, Xm, Ym);
}



//--------------------------------------------------------------------------------------------

// перевод координат из плоской прямоугольной системы в геодезические
void myDMapView :: XmYm_BL(double Xm, double Ym, double& B, double& L)
{
  assert(map_ != NULL && "XmYm_BL");

  map_->XmYm_BL(Xm, Ym, B, L);

  if( L > (M_PI-VERY_SMALL_VALUE) )
    L = L - 2*M_PI;
}
//--------------------------------------------------------------------------------------------

// перевод координат из пикселей картинки в координаты плоской прямоугольной системы
void myDMapView :: XY_XmYm(double X, double Y, double& Xm, double& Ym)
{
  assert(map_ != NULL && "XY_XmYm");

  map_->XY_XmYm(X, Y, Xm, Ym);
}
//--------------------------------------------------------------------------------------------

// перевод координат из плоской прямоугольной ситсемы в пиксели картинки
void myDMapView :: XmYm_XY(double Xm, double Ym, double& X, double& Y)
{
  assert(map_ != NULL && "XmYm_XY");

  map_->XmYm_XY(Xm, Ym, X, Y);
}
//--------------------------------------------------------------------------------------------

// ПОЛУЧЕНИЕ ДАННЫХ С КАРТЫ


// Получение высоты в точке по экранным координатам
double myDMapView :: getHeight_XY(double x, double y)
{
  double Xm, Ym;
  XY_XmYm(x, y, Xm, Ym);
  return getHeight_XmYm(Xm, Ym);
}
//--------------------------------------------------------------------------------------------

// Получение высоты в точке по экранным координатам
bool myDMapView :: getHeight_XY(double x, double y, double &h)
{
  if (GetMapScale() == MAXSCALE_40M)
    return false;

  h = getHeight_XY(x, y);
  return true;
}
//--------------------------------------------------------------------------------------------

// Получение высоты в точке по широте и долготе
double myDMapView :: getHeight_BL(double B, double L)
{
  double Xm, Ym;
  BL_XmYm(B, L, Xm, Ym);
  return getHeight_XmYm(Xm, Ym);
}
//--------------------------------------------------------------------------------------------

// Получение высоты в точке по широте и долготе
bool myDMapView :: getHeight_BL(double B, double L, double &H)
{
  if (GetMapScale() == MAXSCALE_40M)
    return false;

  double Xm, Ym;
  BL_XmYm(B, L, Xm, Ym);
  H = getHeight_XmYm(Xm, Ym);
  return true;
}
//--------------------------------------------------------------------------------------------

// Получение высоты в точке по широте и долготе
void myDMapView :: getHeight_BL(double B, double L, double* H)
{
  double Xm, Ym;
  BL_XmYm(B, L, Xm, Ym);
  *H = getHeight_BL(B, L);
}
//--------------------------------------------------------------------------------------------

// Получение высоты в точке по прямоугольным координатам
double myDMapView :: getHeight_XmYm(double Xm, double Ym)
{
  assert(map_!=NULL && "getHeight_XmYm");

  return map_->getHeight_XmYm(Xm, Ym);
}
//--------------------------------------------------------------------------------------------

// получение магнитного склонения
//возвращает код ошибки:
//0 - нет ошибок, -1 - нет крупномасштабной карты, -2 - нельзя получить магнитное склонение
int myDMapView :: getMagneticAngle_BL(double B, double L, double &Ma)
{
  if (GetMapScale() == MAXSCALE_40M)
    return -1;

  return getMagneticAngle_BL(B, L, &Ma)? 0 : -2;
}
//--------------------------------------------------------------------------------------------

// Получение магнитного склонения
bool myDMapView :: getMagneticAngle_BL(double B, double L, double* Ma)
{
  assert(map_ != NULL && "getMagneticAngle_BL");

  return map_->getMagneticAngle_BL(B, L, Ma);
}
//--------------------------------------------------------------------------------------------

bool myDMapView :: isMtrExist()
{
  assert(map_ != NULL && "isMtrExist");

  return map_->isMtrExist();
}

//--------------------------------------------------------------------------------------------

// ЗАГРУЗКА ИСХОДНЫХ ДАННЫХ

// Установить матрицу высот для открытой карты
void myDMapView :: setMapMtr(QString mtrName)
{
  assert(map_ != NULL && "setMapMtr");

  //    if (TSystemSettings::getHeightMatrixType() == ReliefType_AltitudeElevation)
  //    {
  //        QString mtrName2 = mtrName.replace(QString(".MTW"), QString("_rel.MTW"));
  //        if (QFile::exists(mtrName2))
  //            mtrName = mtrName2;
  //    }

  //    mapLogic->setMapMtr(mtrName);
}
//--------------------------------------------------------------------------------------------

// Открыть карту мира
void myDMapView :: openWorld()
{
  assert(map_ != NULL && "openWorld");

  map_->openWorld();

  //изменение размеров содержимого
  //refreshSize();
  showMap();

#if OS_VERSION == 0 // @TODO@ Временно, сделать нормально
  //SetMapLeftTop(1600, 200); // Россия
#else
  //SetMapLeftTop(1900, 500); // Ирак
#endif
}

//--------------------------------------------------------------------------------------------

bool myDMapView :: isWorldMapOpened()
{
  return map_->getMapFileName().contains("world");
}

// НАСТРОЙКА ПАРАМЕТРОВ ОТОБРАЖЕНИЯ КАРТЫ

// Установить уровень яркости карты (-16  16)
void myDMapView :: SetMapBright(long int value)
{
  assert(map_ != NULL && "SetMapBright");

  map_->setMapBright(value);
}
//--------------------------------------------------------------------------------------------

// Получить уровень яркости
long int myDMapView :: GetMapBright()
{
  assert(map_ !=NULL && "GetMapBright");

  return map_->getMapBright();
}
//--------------------------------------------------------------------------------------------

// Установить уровень контраста изображения
void myDMapView :: SetMapContrast(long int value)
{
  assert(map_ != NULL && "SetMapContrast");

  map_->setMapContrast(value);
}
//--------------------------------------------------------------------------------------------

// Получить текущий уровень контраста
long int myDMapView :: GetMapContrast()
{
  assert(map_ != NULL && "GetMapContrast");

  return map_->getMapContrast();
}
//--------------------------------------------------------------------------------------------

// Установить контурность изображения (true - значит контурная карта)
void myDMapView :: SetMapContour(bool value)
{
  assert(map_ != NULL && "SetMapContour");

  map_->setMapContour(value);
}
//--------------------------------------------------------------------------------------------

// Получить признак контурной карты (true - значит контурная)
bool myDMapView :: GetMapContour()
{
  assert(map_ != NULL && "GetMapContour");

  return map_->getMapContour();
}
//--------------------------------------------------------------------------------------------

// Установить масштаб отображения
void myDMapView :: SetViewScale(float value)
{

}
//--------------------------------------------------------------------------------------------

// Получить масштаб отображения
int myDMapView :: GetViewScale()
{
  assert(map_ != NULL && "GetViewScale");

  return map_->getViewScale();
}


void myDMapView :: GetViewScaleForObjectShaper(int* scale)
{
  *scale = GetViewScale();
}

void myDMapView::on_change()
{
  showMap();
}
//--------------------------------------------------------------------------------------------

// Получить масштаб карты
int myDMapView :: GetMapScale()
{
  assert(map_ != NULL && "GetMapScale");

  return map_->getMapScale();
}
//--------------------------------------------------------------------------------------------
//Увеличивает изображение на текущей карте в два раза
void myDMapView :: ZoomMapIn(double B, double L)
{
  if (GetMapZoom() < MAX_ZOOM)
  {
    SetViewScale(GetViewScale()/2);
    if (B != 0 || L != 0)
    {
      setMapCenter(B, L);
    }

    Repaint();
  }

  //scale(1.1, 1.1);
}

//--------------------------------------------------------------------------------------------
//Уменьшает изображение на текущей карте в два раза
void myDMapView :: ZoomMapOut(double B, double L)
{
  if (GetMapZoom() > MIN_ZOOM)
  {
    uint newView = (GetViewScale())*2;
    if (newView > MAXSCALE_40M)
      newView = MAXSCALE_40M;
    SetViewScale(newView);
    if (B != 0 || L != 0)
    {
      setMapCenter(B, L);
    }

    Repaint();
  }

//  scale(1/1.1, 1/1.1);
}

//--------------------------------------------------------------------------------------------
//Получить текущий коэффициент увеличения
double myDMapView :: GetMapZoom()
{
  double ms = map_->getMapScale();
  double vs = map_->getViewScale();
  return(ms / vs);
}

//--------------------------------------------------------------------------------------------
// Установить геодезические координаты центра отображаемой карты
void myDMapView :: setMapCenter(double centerB, double centerL)
{
  assert(map_ != NULL && "GetMapScale");
  map_->setMapCenter(centerB, centerL);

  double centreX, centreY;
  BL_XY(centerB, centerL, centreX, centreY);
  int left, top;
  left = (int)centreX - viewport()->width()/2;
  top = (int)centreY - viewport()->height()/2;
  SetMapLeftTop(left, top);
}
//--------------------------------------------------------------------------------------------

// Получить центр отображаемой карты в геодезических координатах
void myDMapView :: getMapCenter(double& centerB, double& centerL)
{
  int iLeft, iTop;
  GetMapLeftTop(&iLeft, &iTop);
  iLeft += viewport()->width()/2;
  iTop += viewport()->height()/2;
  XY_BL(iLeft, iTop, centerB, centerL);
}
//--------------------------------------------------------------------------------------------

// Установить центр карты в центр экрана
void myDMapView :: setMapCenterToScreenCenter()
{
  double dCenterB, dCenterL, dCenterX, dCenterY;
  calcCurrentCentreRegion(dCenterB, dCenterL);
  BL_XY(dCenterB, dCenterL, dCenterX, dCenterY);
  int iLeft, iTop;
  iLeft = (int)dCenterX - viewport()->width()/2;
  iTop = (int)dCenterY - viewport()->height()/2;
  SetMapLeftTop(iLeft, iTop);
}
//--------------------------------------------------------------------------------------------

// Рассчитать геодезические координаты центра карты
void myDMapView :: calcCurrentCentreRegion(double &B, double &L)
{
  assert(map_ != NULL && "calcCurrentCentreRegion");

  map_->calcCurrentCentreRegion(B, L);
}
//--------------------------------------------------------------------------------------------

// Установить текущие экранные координаты левого верхнего угла
void myDMapView :: SetMapLeftTop(int left, int top)
{
  //setContentsPos(left, top);
}
//--------------------------------------------------------------------------------------------

// Получить текущие экранные координаты левого верхнего угла
void myDMapView :: GetMapLeftTop(int * left, int * top)
{
  //*left = contentsX();
  //*top = contentsY();
}
//--------------------------------------------------------------------------------------------
void myDMapView :: GetMapRightBottom(int * right, int * bottom)
{
  GetMapLeftTop(right, bottom);
  //*right += viewport()->width();
  //*bottom += viewport()->height();
}


void myDMapView :: getMapLeftTopBL(double& B, double& L)
{
  int left, top;
  GetMapLeftTop(&left, &top);
  XY_BL(left, top, &B, &L);
}

void myDMapView :: getMapRightBottomBL(double& B, double& L)
{
  int right, bottom;
  GetMapRightBottom(&right, &bottom);
  XY_BL(right, bottom, &B, &L);
}

void myDMapView :: setMapLeftTopBL(double B, double L)
{
  int left, top;
  BL_XY(B, L, &left, &top);
  SetMapLeftTop(left, top);
}

//--------------------------------------------------------------------------------------------

// Прочитать и применить настройки карты из файла (яркость, контраст, контур и слои)
void myDMapView :: readSettings(bool restoreSettings)
{
  assert(map_ != NULL && "readSettings");
  map_->readSettings(restoreSettings);
  // InvalidateTask("cartfunc"); //??? в рамках исправления ошибок 2012, 2027
}
//--------------------------------------------------------------------------------------------

// Записать настройки карты в файл (яркость, контраст, контур и слои)
void myDMapView :: writeSettings()
{
  assert(map_ != NULL && "writeSettings");

  map_->writeSettings();
}
//--------------------------------------------------------------------------------------------

// Записать настройки предыдущей карты в файл (яркость, контраст, контур и слои)
void myDMapView :: writePreviousSettings()
{
  assert(map_ != NULL && "writePreviousSettings");

  map_->writePreviousSettings();
}
//--------------------------------------------------------------------------------------------

// Установить видимость слоя по индексу (после установок необходимо применить изменения setViewSelect() )
void myDMapView :: selectLayer(int index, bool selected)
{
  assert(map_ != NULL && "selectLayer");

  map_->selectLayer(index, selected);
}
//--------------------------------------------------------------------------------------------
void myDMapView :: selectObject(long inCode, bool selected)
{
  assert(map_ != NULL && "selectObject");

  map_->selectObject(inCode, selected);
}
//--------------------------------------------------------------------------------------------

// Получить признак видимости слоя по индексу
bool myDMapView :: isLayerSelected(int index)
{
  assert(map_ != NULL && "isLayerSelected");

  return map_->isLayerSelected(index);
}
//--------------------------------------------------------------------------------------------
bool myDMapView :: isObjectSelected(long inCode)
{
  assert(map_ != NULL && "isObjectSelected");

  return map_->isObjectSelected(inCode);
}
//--------------------------------------------------------------------------------------------

// Получить количество всех слоев
int myDMapView :: getLayersCount()
{
  assert(map_ != NULL && "getLayersCount");

  return map_->getLayersCount();
}
//--------------------------------------------------------------------------------------------

// Получить количество объектов карты
int myDMapView :: getObjectsCount()
{
  assert(map_ != NULL && "getObjectsCount");

  return map_->getObjectsCount();
}

int myDMapView :: getObjectsCountInLayer(int layerIndex)
{
  assert(map_ != NULL && "getObjectsCountInLayer");

  return map_->getObjectsCountInLayer(layerIndex);
}

//Получить имя объекта слоя по порядковому номеру
QString myDMapView :: getLayerObjectName(int layerIndex, int objectIndex)
{
  assert(map_ != NULL && "getLayerObjectName");

  return map_->getLayerObjectName(layerIndex, objectIndex);
}

//Получить код объекта слоя по порядковому номеру
long myDMapView :: getLayerObjectCode(int layerIndex, int objectIndex)
{
  assert(map_ != NULL && "getLayerObjectCode");

  return map_->getLayerObjectCode(layerIndex, objectIndex);
}

//Получить индекс слоя по номеру объекта
int myDMapView :: getLayerIndex(long inCode)
{
  assert(map_ != NULL && "getLayerIndex");

  return map_->getLayerIndex(inCode);
}

//Получить внутренние номера всех объектов слоя
QList<long> myDMapView :: getLayerObjectsCodes(int layerIndex)
{
  assert(map_ != NULL && "getLayerObjectsCodes");

  return map_->getLayerObjectsCodes(layerIndex);
}

// Получить имя слоя по индексу
QString myDMapView :: getMapLayerName(int index)
{
  assert(map_ != NULL && "getMapLayerName");

  return map_->getMapLayerName(index);
}
//--------------------------------------------------------------------------------------------
QString myDMapView :: getMapObjectName(int index)
{
  assert(map_ != NULL && "getMapObjectName");

  return map_->getMapObjectName(index);
}
//--------------------------------------------------------------------------------------------

// Получить имя видимого слоя по индексу видимых слоев
QString myDMapView :: getMapVisibleLayerName(int index)
{
  assert(map_ != NULL && "getMapVisibleLayerName");

  return map_->getMapVisibleLayerName(index);
}
//--------------------------------------------------------------------------------------------
QString myDMapView :: getMapVisibleObjectName(int index)
{
  assert(map_ != NULL && "getMapVisibleObjectName");

  return map_->getMapVisibleObjectName(index);
}
//--------------------------------------------------------------------------------------------

// Получить количество видимых слоев
int myDMapView :: getVisibleLayersCount()
{
  assert(map_ != NULL && "getVisibleLayersCount");

  return map_->getVisibleLayersCount();
}
//--------------------------------------------------------------------------------------------
int myDMapView :: getVisibleObjectsCount()
{
  assert(map_ != NULL && "getVisibleObjectsCount");

  return map_->getVisibleObjectsCount();
}
//--------------------------------------------------------------------------------------------

// Применить изменения связанные со слоями к открытой карте
void myDMapView :: setViewSelect()
{
  assert(map_ != NULL && "setViewSelect");

  map_->setViewSelect();
  Repaint();
}
//--------------------------------------------------------------------------------------------


// ОТРИСОВКА

// Получить изображение карты
long int myDMapView :: getImageMap(int left, int top, int width, int height, QImage& image)
{
  assert(map_ != NULL && "getImageMap");
  return map_->getImageMap(left, top, width, height, image);
}
//--------------------------------------------------------------------------------------------

QPixmap myDMapView :: getCurrentPixmap()
{
  return isTaskPixmapFilled ? task_currentPixmapMapViewPort:old_currentPixmapMapViewPort;
}

//--------------------------------------------------------------------------------------------

// Перерисовать контент
void myDMapView :: Repaint()
{

}

void myDMapView::showMap()
{

}

// Установить задачу для отрисовки
void myDMapView :: SetDrawTask(QString nameTask, TDrawData _draw, vector<QString> ListTasks)
{
  DeleteTaskDraw(nameTask);
  mapTaskDraw[nameTask] = _draw._dataDraw;
  mapListTaskInvalidateForTask[nameTask] = ListTasks;

  vector<QString> _vectMapTask = mapListTaskInvalidateForTask["Map"];
  if(std::find(_vectMapTask.begin(), _vectMapTask.end(), nameTask) == _vectMapTask.end())
    mapListTaskInvalidateForTask["Map"].push_back(nameTask);
  DrawTask("Map");
}
//--------------------------------------------------------------------------------------------

// Выполнить задачу отрисовки (и связанные с ней)
void myDMapView :: DrawTask(QString nameTask)
{
  /*
 if(old_currentPixmapMapViewPort.width() <= 0 || old_currentPixmapMapViewPort.height() <= 0)
  return;

 isTaskPixmapFilled = false;

 if(it_activeDrawTask != mapTaskDraw.find(nameTask) || "Map" == nameTask)
  {
     if("Map" != nameTask)
    it_activeDrawTask = mapTaskDraw.find(nameTask);
   if(it_activeDrawTask != mapTaskDraw.end())
    {
     copyBlt(&task_currentPixmapMapViewPort, 0, 0, &_currentPixmapMapViewPort, 0, 0, task_currentPixmapMapViewPort.width(), task_currentPixmapMapViewPort.height());
         isTaskPixmapFilled = true;
     set<QString > _drawTask;
     _drawTask.clear();
     _drawTask.insert(it_activeDrawTask->first);
     for(unsigned int i = 0; i < mapListTaskInvalidateForTask[it_activeDrawTask->first].size(); i++)
      _drawTask.insert(mapListTaskInvalidateForTask[it_activeDrawTask->first][i]);

     QPainter _p;
     _p.begin(&task_currentPixmapMapViewPort);
     _p.translate(-contentsX(), -contentsY());
         vector<QString>::iterator it = mapListTaskInvalidateForTask["Map"].begin();
         while(it != mapListTaskInvalidateForTask["Map"].end())
      {
       DrawTask(*it, _drawTask, &_p);
       it++;
      }
     _p.end();
    }
  }

 if(it_activeDrawTask != mapTaskDraw.end())
  {
   set<QString > _drawTask;
   _drawTask.clear();

   copyBlt(&old_currentPixmapMapViewPort, 0, 0, &task_currentPixmapMapViewPort, 0, 0, old_currentPixmapMapViewPort.width(), old_currentPixmapMapViewPort.height());
     isTaskPixmapFilled = true;
   QPainter _p;
   _p.begin(&old_currentPixmapMapViewPort);
   _p.translate(-contentsX(), -contentsY());
   DrawTask(it_activeDrawTask->first, _drawTask, &_p);
   _p.end();
  }

 if("Map" == nameTask && it_activeDrawTask == mapTaskDraw.end())
  {
   copyBlt(&old_currentPixmapMapViewPort, 0, 0, &_currentPixmapMapViewPort, 0, 0, old_currentPixmapMapViewPort.width(), old_currentPixmapMapViewPort.height());
     vector<QString>::iterator it = mapListTaskInvalidateForTask["Map"].begin();

   set<QString > _drawTask;
   _drawTask.clear();

   QPainter _p;
   _p.begin(&old_currentPixmapMapViewPort);
   _p.translate(-contentsX(), -contentsY());
   while(it != mapListTaskInvalidateForTask[nameTask].end())
    {
     DrawTask(*it, _drawTask, &_p);
     it++;
    }
   _p.end();
  }
    updateContents();
    */
}
//--------------------------------------------------------------------------------------------

void myDMapView::DrawTask(QString nameTask, set<QString >& _drawTask, QPainter* p)
{
  if(_drawTask.find(nameTask) == _drawTask.end() && mapTaskDraw.find(nameTask) != mapTaskDraw.end())
  {
    _drawTask.insert(nameTask);
    mapTaskDraw[nameTask]->Draw(p);

    vector<QString>::iterator it = mapListTaskInvalidateForTask[nameTask].begin();
    while(it != mapListTaskInvalidateForTask[nameTask].end())
    {
      DrawTask(*it, _drawTask, p);
      it++;
    }
  }
}

void myDMapView::showTextHint()
{
  auto imageLT = graphicsItemMap_->imageLT();
  auto p = imageLT + mousePos_;
  double b = 0, l = 0;
  if(map_)
    map_->XY_BL(p.x(), p.y(), b, l);

  textItem_->setPos(0, 0);
  textItem_->setText("Map B: " + QString().setNum(b) + ", L: " + QString().setNum(l));
}
//--------------------------------------------------------------------------------------------

// Выполнить задачу отрисовки (и связанные с ней)
void myDMapView :: InvalidateTask(QString nameTask)
{
  //DrawTask(nameTask);
  //viewport()->repaint(false);
}
//--------------------------------------------------------------------------------------------

// удалить задачу отрисовки
void myDMapView :: DeleteTaskDraw(QString nameTask)
{
  if(mapTaskDraw.find(nameTask) != mapTaskDraw.end())
  {
    if(it_activeDrawTask == mapTaskDraw.find(nameTask))
      it_activeDrawTask = mapTaskDraw.end();

    if(mapTaskDraw[nameTask])
      delete mapTaskDraw[nameTask];
    mapTaskDraw.erase(nameTask);
  }
  mapListTaskInvalidateForTask.erase(nameTask);
  vector<QString>& _vectMapTask = mapListTaskInvalidateForTask["Map"];
  vector<QString> :: iterator it = std::find(_vectMapTask.begin(), _vectMapTask.end(), nameTask);
  if(it != _vectMapTask.end())
  {
    mapListTaskInvalidateForTask["Map"].erase(it);
  }
  DrawTask("Map");
}
//--------------------------------------------------------------------------------------------

// РАБОТА С МЫШКОЙ

// Добавить или сделать текущей задачу работы с мышкой
void myDMapView :: SetActiveMouseTask(QString value)
{
  vector<QString >::iterator it = std::find(vectTaskMouse.begin(), vectTaskMouse.end(), value);
  if(it != vectTaskMouse.end())
  {
    if(vectTaskMouse.back() != value)
    {
      vectTaskMouse.erase(it);
      vectTaskMouse.push_back(value);
    }
  }
  else
    vectTaskMouse.push_back(value);
}
//--------------------------------------------------------------------------------------------

// Получить текущую задачу рабты с мышкой
QString myDMapView :: GetActiveMouseTask(void)
{
  if(vectTaskMouse.size() > 0)
    return vectTaskMouse.back();
  return "";
}
//--------------------------------------------------------------------------------------------

// Удалить задачу работы с мышкой
void myDMapView :: DeleteMouseTask(QString value)
{
  vector<QString >::iterator it = std::find(vectTaskMouse.begin(), vectTaskMouse.end(), value);
  if(it != vectTaskMouse.end())
    vectTaskMouse.erase(it);
}
//--------------------------------------------------------------------------------------------

bool myDMapView :: isMouseTaskExist(QString value)
{
  vector<QString >::iterator it = std::find(vectTaskMouse.begin(), vectTaskMouse.end(), value);
  return (it != vectTaskMouse.end());
}

void myDMapView :: mousePressEvent(QMouseEvent* e)
{
  int x = e->x();
  int y = e->y();

  //double height = getHeight_XY(x, y);

  double B, L;
  XY_BL(x, y, B, L);

  //emit MouseDown(GetActiveMouseTask(), e->x(), e->y(), x, y, B, L, height, e->button(), e->state());

  QGraphicsView::mousePressEvent(e);
}

//--------------------------------------------------------------------------------------------

void myDMapView :: mouseMoveEvent(QMouseEvent* e)
{
  mousePos_ = e->pos();
  showTextHint();

  int x = e->x();
  int y = e->y();

  //double height = getHeight_XY(x, y);

  double B, L;
  XY_BL(x, y, B, L);

  //emit MouseMove(GetActiveMouseTask(), e->x(), e->y(), x, y, B, L, height, e->button(), e->state());

  QGraphicsView::mouseMoveEvent(e);
}
//--------------------------------------------------------------------------------------------

void myDMapView :: mouseReleaseEvent(QMouseEvent* e)
{
  int x = e->x();
  int y = e->y();

  //double height = getHeight_XY(x, y);

  double B, L;
  XY_BL(x, y, B, L);

  //emit MouseUp(GetActiveMouseTask(), e->x(), e->y(), x, y, B, L, height, e->button(), e->state());

  QGraphicsView::mouseReleaseEvent(e);
}
//--------------------------------------------------------------------------------------------

void myDMapView :: keyPressEvent(QKeyEvent * keyEvent)
{
  int movingFactor = 1;
  if(keyEvent->modifiers() == Qt::AltModifier)
    movingFactor = 10;

  switch (keyEvent->key())
  {
    case Qt::Key_Left:
    {
      QPoint cursorPos = cursor().pos();
      cursorPos.setX(cursorPos.x() - movingFactor);
      cursor().setPos(cursorPos);
    }break;
    case Qt::Key_Right:
    {
      QPoint cursorPos = cursor().pos();
      cursorPos.setX(cursorPos.x() + movingFactor);
      cursor().setPos(cursorPos);

    }break;
    case Qt::Key_Up:
    {
      QPoint cursorPos = cursor().pos();
      cursorPos.setY(cursorPos.y() - movingFactor);
      cursor().setPos(cursorPos);
    }break;
    case Qt::Key_Down:
    {
      QPoint cursorPos = cursor().pos();
      cursorPos.setY(cursorPos.y() + movingFactor);
      cursor().setPos(cursorPos);
    }break;
    case Qt::Key_Return:
    {
      int x, y;
      //this->viewportToContents(mapFromGlobal(cursor().pos()).x(), mapFromGlobal(cursor().pos()).y(), x, y);

      double height = getHeight_XY(x, y);
      double B, L;
      XY_BL(x, y, B, L);

      if(keyEvent->modifiers() == Qt::NoModifier)
      {
        //emit MouseDown(GetActiveMouseTask(),x, y, x, y, B, L, height, Qt::LeftButton, Qt::NoModifier);
        //emit MouseUp(GetActiveMouseTask(), x, y, x, y, B, L, height, Qt::LeftButton, Qt::NoModifier);
      }
      //else if (keyEvent->modifiers() == Qt::AltModifier)
      //emit MouseDown(GetActiveMouseTask(),x, y, x, y, B, L, height, Qt::RightButton, Qt::AltModifier);
    }break;
    default: break;
  }
  if (keyEvent->key() == Qt::Key_Escape)
    emit escPressedSignal();

  if ( keyEvent->modifiers() == Qt::ShiftModifier && (keyEvent->key() == Qt::Key_Z || keyEvent->key() == 1071) )
  {
    if (isKeyboardGrabbed())
      releaseMapViewKeyboard();
    else
      grabMapViewKeyboard();
  }

  //if (keyEvent->key() == Qt::Key_8)
  //verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
  //else if (keyEvent->key() == Qt::Key_2)
  //verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);
  //else if (keyEvent->key() == Qt::Key_4)
  //horizontalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
  //else if (keyEvent->key() == Qt::Key_6)
  //horizontalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);

  QGraphicsView::keyPressEvent(keyEvent);

}

void myDMapView :: resizeEvent(QResizeEvent* e)
{
  scene()->setSceneRect(0, 0, viewport()->width(), viewport()->height());
  graphicsItemMap_->resize(viewport()->width(), viewport()->height());

  QGraphicsView::resizeEvent(e);
}

void myDMapView::wheelEvent(QWheelEvent *e)
{
//  if(e->modifiers().testFlag(Qt::ControlModifier))
//  {
//    QPoint numPixels = e->pixelDelta();
//    QPoint numDegrees = e->angleDelta() / 8;

//    if(!numPixels.isNull())
//    {
//      e->accept();
//      //qDebug() << "wheel1 x: " << numPixels.x() << " wheel1 y: " << numPixels.y();
//      return;
//    }
//    else if(!numDegrees.isNull())
//    {
//      e->accept();
//      QPoint numSteps = numDegrees / 15;
//      if(numSteps.y() > 0)
//        scale(1.1, 1.1);
//      else
//        scale(1/1.1, 1/1.1);
//      //qDebug() << "wheel2 x: " << numSteps.x() << " wheel2 y: " << numSteps.y();
//      return;
//    }
//  }

  QGraphicsView::wheelEvent(e);
}
//--------------------------------------------------------------------------------------------


void myDMapView :: grabMapViewKeyboard()
{
  grabKeyboard();
  keyboardGrabbed = true;
}

void myDMapView :: releaseMapViewKeyboard()
{
  releaseKeyboard();
  keyboardGrabbed = false;
}

bool myDMapView :: isKeyboardGrabbed()
{
  return keyboardGrabbed;
}

void myDMapView::paintEvent(QPaintEvent *pe)
{

  QGraphicsView::paintEvent(pe);
}
