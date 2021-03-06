//	���� ���� �������� ������ ������� �� "�����������".
//	����� ��������: ��� ��� 7 13:24:30 2007
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
#include <QtDebug>

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

myDMapView :: myDMapView(QWidget *parent) : QGraphicsView(parent), mapLogic(NULL)
{
  this->setObjectName("MapView");
  //viewport()->setMouseTracking(true);

  setScene(&scene_);

  mapTaskDraw.clear();
  mapListTaskInvalidateForTask.clear();
  it_activeDrawTask = mapTaskDraw.end();
  _cx = -1;
  _cy = -1;
  _cw = -1;
  _ch = -1;
  _oldScale = 0;
  _contentsX = 0;
  _contentsY = 0;

  keyboardGrabbed = false;
  isTaskPixmapFilled = false;

  //viewport()->installEventFilter(this);
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
  mapLogic = map;
}

// ������� ���������

// ������� ��������� �� ������������� � ������� �������� �����
void myDMapView :: BL_XY(double B, double L, double& X, double& Y)
{
  double Xm, Ym;
  BL_XmYm(B, L, Xm, Ym);
  XmYm_XY(Xm, Ym, X, Y);
}
//--------------------------------------------------------------------------------------------

// ������� ��������� �� �������� �������� ����� � �������������
void myDMapView :: XY_BL(double X, double Y, double& B, double& L)
{
  double Xm, Ym;
  XY_XmYm(X, Y, Xm, Ym);
  XmYm_BL(Xm, Ym, B, L);

  if( L > (M_PI - VERY_SMALL_VALUE) )
    L = L - 2*M_PI;
}
//--------------------------------------------------------------------------------------------

// ������� ��������� �� ������������� � ������� �������� �����
void myDMapView :: BL_XY(double B, double L, int* X, int* Y)
{
  double tempX;
  double tempY;

  BL_XY(B, L, tempX, tempY);

  *X = (int) tempX;
  *Y = (int) tempY;
}

// ������� ��������� �� �������� �������� ����� � �������������	
void myDMapView :: XY_BL(int X, int Y, double* B, double* L)
{
  XY_BL((double) X, (double) Y, *B, *L);
}

//������� ������������� ���������  � ���������� ������������� ������� �������
void myDMapView :: BL_XmYm(double B, double L, double& Xm, double& Ym)
{
  assert(mapLogic != NULL && "BL_XmYm");

  mapLogic->BL_XmYm(B, L, Xm, Ym);
}



//--------------------------------------------------------------------------------------------

// ������� ��������� �� ������� ������������� ������� � �������������
void myDMapView :: XmYm_BL(double Xm, double Ym, double& B, double& L)
{
  assert(mapLogic != NULL && "XmYm_BL");

  mapLogic->XmYm_BL(Xm, Ym, B, L);

  if( L > (M_PI-VERY_SMALL_VALUE) )
    L = L - 2*M_PI;
}
//--------------------------------------------------------------------------------------------

// ������� ��������� �� �������� �������� � ���������� ������� ������������� �������
void myDMapView :: XY_XmYm(double X, double Y, double& Xm, double& Ym)
{
  assert(mapLogic != NULL && "XY_XmYm");

  mapLogic->XY_XmYm(X, Y, Xm, Ym);
}
//--------------------------------------------------------------------------------------------

// ������� ��������� �� ������� ������������� ������� � ������� ��������
void myDMapView :: XmYm_XY(double Xm, double Ym, double& X, double& Y)
{
  assert(mapLogic != NULL && "XmYm_XY");

  mapLogic->XmYm_XY(Xm, Ym, X, Y);
}
//--------------------------------------------------------------------------------------------

// ��������� ������ � �����


// ��������� ������ � ����� �� �������� �����������
double myDMapView :: getHeight_XY(double x, double y)
{
  double Xm, Ym;
  XY_XmYm(x, y, Xm, Ym);
  return getHeight_XmYm(Xm, Ym);
}
//--------------------------------------------------------------------------------------------

// ��������� ������ � ����� �� �������� �����������
bool myDMapView :: getHeight_XY(double x, double y, double &h)
{
  if (GetMapScale() == MAXSCALE_40M)
    return false;

  h = getHeight_XY(x, y);
  return true;
}
//--------------------------------------------------------------------------------------------

// ��������� ������ � ����� �� ������ � �������
double myDMapView :: getHeight_BL(double B, double L)
{
  double Xm, Ym;
  BL_XmYm(B, L, Xm, Ym);
  return getHeight_XmYm(Xm, Ym);
}
//--------------------------------------------------------------------------------------------

// ��������� ������ � ����� �� ������ � �������
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

// ��������� ������ � ����� �� ������ � �������
void myDMapView :: getHeight_BL(double B, double L, double* H)
{
  double Xm, Ym;
  BL_XmYm(B, L, Xm, Ym);
  *H = getHeight_BL(B, L);
}
//--------------------------------------------------------------------------------------------

// ��������� ������ � ����� �� ������������� �����������
double myDMapView :: getHeight_XmYm(double Xm, double Ym)
{
  assert(mapLogic!=NULL && "getHeight_XmYm");

  return mapLogic->getHeight_XmYm(Xm, Ym);
}
//--------------------------------------------------------------------------------------------

// ��������� ���������� ���������
//���������� ��� ������:
//0 - ��� ������, -1 - ��� ���������������� �����, -2 - ������ �������� ��������� ���������
int myDMapView :: getMagneticAngle_BL(double B, double L, double &Ma)
{
  if (GetMapScale() == MAXSCALE_40M)
    return -1;

  return getMagneticAngle_BL(B, L, &Ma)? 0 : -2;
}
//--------------------------------------------------------------------------------------------

// ��������� ���������� ���������
bool myDMapView :: getMagneticAngle_BL(double B, double L, double* Ma)
{
  assert(mapLogic != NULL && "getMagneticAngle_BL");

  return mapLogic->getMagneticAngle_BL(B, L, Ma);
}
//--------------------------------------------------------------------------------------------

bool myDMapView :: isMtrExist()
{
  assert(mapLogic != NULL && "isMtrExist");

  return mapLogic->isMtrExist();
}

//--------------------------------------------------------------------------------------------

// �������� �������� ������

// ������� ����� �� ����� �����
void myDMapView :: SetMapFileName(QString mapName)
{
  assert(mapLogic != NULL && "SetMapFileName");
  if (mapLogic->setMap(mapName))
  {
    refreshSize();
    showMap();
  }
}
//--------------------------------------------------------------------------------------------     

// �������� ��� �������� �����
QString myDMapView::GetMapFileName()
{
  assert(mapLogic != NULL && "GetMapFileName");

  return mapLogic->getMapFileName();
}
//--------------------------------------------------------------------------------------------

// ���������� ������� ����� ��� �������� �����
void myDMapView :: setMapMtr(QString mtrName)
{
  assert(mapLogic != NULL && "setMapMtr");

  //    if (TSystemSettings::getHeightMatrixType() == ReliefType_AltitudeElevation)
  //    {
  //        QString mtrName2 = mtrName.replace(QString(".MTW"), QString("_rel.MTW"));
  //        if (QFile::exists(mtrName2))
  //            mtrName = mtrName2;
  //    }

  //    mapLogic->setMapMtr(mtrName);
}
//--------------------------------------------------------------------------------------------

// ������� ����� ����    
void myDMapView :: openWorld()
{
  assert(mapLogic != NULL && "openWorld");

  mapLogic->openWorld();

  //��������� �������� �����������
  refreshSize();
  showMap();

#if OS_VERSION == 0 // @TODO@ ��������, ������� ���������
  //SetMapLeftTop(1600, 200); // ������
#else
  //SetMapLeftTop(1900, 500); // ����
#endif
}

//--------------------------------------------------------------------------------------------

bool myDMapView :: isWorldMapOpened()
{
  return GetMapFileName().contains("world");
}

//--------------------------------------------------------------------------------------------

// ��������� ���������� ����������� �����

// ���������� ������� ������� ����� (-16  16)
void myDMapView :: SetMapBright(long int value)
{
  assert(mapLogic != NULL && "SetMapBright");

  mapLogic->setMapBright(value);
}
//--------------------------------------------------------------------------------------------

// �������� ������� �������
long int myDMapView :: GetMapBright()
{
  assert(mapLogic !=NULL && "GetMapBright");

  return mapLogic->getMapBright();
}
//--------------------------------------------------------------------------------------------

// ���������� ������� ��������� �����������
void myDMapView :: SetMapContrast(long int value)
{
  assert(mapLogic != NULL && "SetMapContrast");

  mapLogic->setMapContrast(value);
}
//--------------------------------------------------------------------------------------------

// �������� ������� ������� ��������� 
long int myDMapView :: GetMapContrast()
{
  assert(mapLogic != NULL && "GetMapContrast");

  return mapLogic->getMapContrast();
}
//--------------------------------------------------------------------------------------------

// ���������� ����������� ����������� (true - ������ ��������� �����)    
void myDMapView :: SetMapContour(bool value)
{
  assert(mapLogic != NULL && "SetMapContour");

  mapLogic->setMapContour(value);
}
//--------------------------------------------------------------------------------------------

// �������� ������� ��������� ����� (true - ������ ���������)   
bool myDMapView :: GetMapContour()
{
  assert(mapLogic != NULL && "GetMapContour");

  return mapLogic->getMapContour();
}
//--------------------------------------------------------------------------------------------

// ���������� ������� �����������    
void myDMapView :: SetViewScale(float value)
{
  assert(mapLogic != NULL && "SetViewScale");

  long int X,Y;

  //�������� ������� �����
  //X = contentsX()+ visibleRegion()/2;
  //Y=contentsY()+ visibleHeight()/2;
  X = _cw /2;
  Y = _ch /2;

  mapLogic->setViewScale(X,Y,value);

  //long int width, height;
  //mapLogic->getMapImageSize(width, height);
  //��������� �������� �����������
  //resizeContents(width, height);
  //resize(width, height);

  //�������� ����� �����
  //X=X-visibleWidth()/2;
  //if(X<0) X=0;
  //Y=Y-visibleHeight()/2;
  //if(Y<0) Y=0;
  //setContentsPos(X,Y);

  //const int x = horizontalScrollBar()->value() + dx;
  //horizontalScrollBar()->setValue( x );

  //const int y = verticalScrollBar()->value() + dy;
  //verticalScrollBar()->setValue( y );
}
//--------------------------------------------------------------------------------------------

// �������� ������� �����������  
int myDMapView :: GetViewScale()
{
  assert(mapLogic != NULL && "GetViewScale");

  return mapLogic->getViewScale();
}


void myDMapView :: GetViewScaleForObjectShaper(int* scale)
{
  *scale = GetViewScale();
}
//--------------------------------------------------------------------------------------------

// �������� ������� ����� 
int myDMapView :: GetMapScale()
{
  assert(mapLogic != NULL && "GetMapScale");

  return mapLogic->getMapScale();
}
//--------------------------------------------------------------------------------------------
//����������� ����������� �� ������� ����� � ��� ����
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
//��������� ����������� �� ������� ����� � ��� ����
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
//�������� ������� ����������� ����������
double myDMapView :: GetMapZoom()
{
  double ms = mapLogic->getMapScale();
  double vs = mapLogic->getViewScale();
  return(ms / vs);
}

//--------------------------------------------------------------------------------------------
// ���������� ������������� ���������� ������ ������������ �����
void myDMapView :: setMapCenter(double centerB, double centerL)
{
  assert(mapLogic != NULL && "GetMapScale");
  mapLogic->setMapCenter(centerB, centerL);

  double centreX, centreY;
  BL_XY(centerB, centerL, centreX, centreY);
  int left, top;
  left = (int)centreX - viewport()->width()/2;
  top = (int)centreY - viewport()->height()/2;
  SetMapLeftTop(left, top);
}
//--------------------------------------------------------------------------------------------

// �������� ����� ������������ ����� � ������������� �����������
void myDMapView :: getMapCenter(double& centerB, double& centerL)
{
  int iLeft, iTop;
  GetMapLeftTop(&iLeft, &iTop);
  iLeft += viewport()->width()/2;
  iTop += viewport()->height()/2;
  XY_BL(iLeft, iTop, centerB, centerL);
}
//--------------------------------------------------------------------------------------------

// ���������� ����� ����� � ����� ������    
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

// ���������� ������������� ���������� ������ �����  
void myDMapView :: calcCurrentCentreRegion(double &B, double &L)
{
  assert(mapLogic != NULL && "calcCurrentCentreRegion");

  mapLogic->calcCurrentCentreRegion(B, L);
}
//--------------------------------------------------------------------------------------------

// ���������� ������� �������� ���������� ������ �������� ����
void myDMapView :: SetMapLeftTop(int left, int top)
{
  //setContentsPos(left, top);
}
//--------------------------------------------------------------------------------------------

// �������� ������� �������� ���������� ������ �������� ����     
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

// ��������� � ��������� ��������� ����� �� ����� (�������, ��������, ������ � ����) 
void myDMapView :: readSettings(bool restoreSettings)
{
  assert(mapLogic != NULL && "readSettings");
  mapLogic->readSettings(restoreSettings);
  // InvalidateTask("cartfunc"); //??? � ������ ����������� ������ 2012, 2027
}
//--------------------------------------------------------------------------------------------

// �������� ��������� ����� � ���� (�������, ��������, ������ � ����)   
void myDMapView :: writeSettings()
{
  assert(mapLogic != NULL && "writeSettings");

  mapLogic->writeSettings();
}
//--------------------------------------------------------------------------------------------

// �������� ��������� ���������� ����� � ���� (�������, ��������, ������ � ����)
void myDMapView :: writePreviousSettings()
{
  assert(mapLogic != NULL && "writePreviousSettings");

  mapLogic->writePreviousSettings();
}
//--------------------------------------------------------------------------------------------

// ���������� ��������� ���� �� ������� (����� ��������� ���������� ��������� ��������� setViewSelect() ) 
void myDMapView :: selectLayer(int index, bool selected)
{
  assert(mapLogic != NULL && "selectLayer");

  mapLogic->selectLayer(index, selected);
}
//--------------------------------------------------------------------------------------------
void myDMapView :: selectObject(long inCode, bool selected)
{
  assert(mapLogic != NULL && "selectObject");

  mapLogic->selectObject(inCode, selected);
}
//--------------------------------------------------------------------------------------------

// �������� ������� ��������� ���� �� �������
bool myDMapView :: isLayerSelected(int index)
{
  assert(mapLogic != NULL && "isLayerSelected");

  return mapLogic->isLayerSelected(index);
}
//--------------------------------------------------------------------------------------------
bool myDMapView :: isObjectSelected(long inCode)
{
  assert(mapLogic != NULL && "isObjectSelected");

  return mapLogic->isObjectSelected(inCode);
}
//--------------------------------------------------------------------------------------------

// �������� ���������� ���� �����  
int myDMapView :: getLayersCount()
{
  assert(mapLogic != NULL && "getLayersCount");

  return mapLogic->getLayersCount();
}
//--------------------------------------------------------------------------------------------

// �������� ���������� �������� �����
int myDMapView :: getObjectsCount()
{
  assert(mapLogic != NULL && "getObjectsCount");

  return mapLogic->getObjectsCount();
}

int myDMapView :: getObjectsCountInLayer(int layerIndex)
{
  assert(mapLogic != NULL && "getObjectsCountInLayer");

  return mapLogic->getObjectsCountInLayer(layerIndex);
}

//�������� ��� ������� ���� �� ����������� ������
QString myDMapView :: getLayerObjectName(int layerIndex, int objectIndex)
{
  assert(mapLogic != NULL && "getLayerObjectName");

  return mapLogic->getLayerObjectName(layerIndex, objectIndex);
}

//�������� ��� ������� ���� �� ����������� ������
long myDMapView :: getLayerObjectCode(int layerIndex, int objectIndex)
{
  assert(mapLogic != NULL && "getLayerObjectCode");

  return mapLogic->getLayerObjectCode(layerIndex, objectIndex);
}

//�������� ������ ���� �� ������ �������
int myDMapView :: getLayerIndex(long inCode)
{
  assert(mapLogic != NULL && "getLayerIndex");

  return mapLogic->getLayerIndex(inCode);
}

//�������� ���������� ������ ���� �������� ����
QList<long> myDMapView :: getLayerObjectsCodes(int layerIndex)
{
  assert(mapLogic != NULL && "getLayerObjectsCodes");

  return mapLogic->getLayerObjectsCodes(layerIndex);
}

// �������� ��� ���� �� �������
QString myDMapView :: getMapLayerName(int index)
{
  assert(mapLogic != NULL && "getMapLayerName");

  return mapLogic->getMapLayerName(index);
}
//--------------------------------------------------------------------------------------------
QString myDMapView :: getMapObjectName(int index)
{
  assert(mapLogic != NULL && "getMapObjectName");

  return mapLogic->getMapObjectName(index);
}
//--------------------------------------------------------------------------------------------

// �������� ��� �������� ���� �� ������� ������� �����   
QString myDMapView :: getMapVisibleLayerName(int index)
{
  assert(mapLogic != NULL && "getMapVisibleLayerName");

  return mapLogic->getMapVisibleLayerName(index);
}
//--------------------------------------------------------------------------------------------
QString myDMapView :: getMapVisibleObjectName(int index)
{
  assert(mapLogic != NULL && "getMapVisibleObjectName");

  return mapLogic->getMapVisibleObjectName(index);
}
//--------------------------------------------------------------------------------------------

// �������� ���������� ������� �����    
int myDMapView :: getVisibleLayersCount()
{
  assert(mapLogic != NULL && "getVisibleLayersCount");

  return mapLogic->getVisibleLayersCount();
}
//--------------------------------------------------------------------------------------------
int myDMapView :: getVisibleObjectsCount()
{
  assert(mapLogic != NULL && "getVisibleObjectsCount");

  return mapLogic->getVisibleObjectsCount();
}
//--------------------------------------------------------------------------------------------

// ��������� ��������� ��������� �� ������ � �������� �����    
void myDMapView :: setViewSelect()
{
  assert(mapLogic != NULL && "setViewSelect");

  mapLogic->setViewSelect();
  Repaint();
}
//--------------------------------------------------------------------------------------------


// ���������

// �������� ����������� �����
long int myDMapView :: getImageMap(int left, int top, int width, int height, QImage& image)
{
  assert(mapLogic != NULL && "getImageMap");
  return mapLogic->getImageMap(left, top, width, height, image);
}
//--------------------------------------------------------------------------------------------

QPixmap myDMapView :: getCurrentPixmap()
{
  return isTaskPixmapFilled ? task_currentPixmapMapViewPort:old_currentPixmapMapViewPort;
}

//--------------------------------------------------------------------------------------------

// ������������ �������
void myDMapView :: Repaint()
{
  QPainter p(this);
  drawContents(&p, x(), y(), width(), height());
}

void myDMapView::showMap()
{
  Repaint();

  centerOn(_cw/2, _ch/2);
}
//--------------------------------------------------------------------------------------------

void myDMapView :: badRepaint()
{
  //viewport()->setGeometry(viewport()->x(), viewport()->y(), viewport()->width(), viewport()->height()-1);
  //viewport()->repaint(false);
  //viewport()->setGeometry(viewport()->x(), viewport()->y(), viewport()->width(), viewport()->height()+1);
}

// ���������� ������ ��� ���������
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

// ��������� ������ ��������� (� ��������� � ���)
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
//--------------------------------------------------------------------------------------------

// ��������� ������ ��������� (� ��������� � ���)
void myDMapView :: InvalidateTask(QString nameTask)
{
  //DrawTask(nameTask);
  //viewport()->repaint(false);
}
//--------------------------------------------------------------------------------------------

// ������� ������ ���������
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

// ������ � ������   

// �������� ��� ������� ������� ������ ������ � ������
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

// �������� ������� ������ ����� � ������
QString myDMapView :: GetActiveMouseTask(void)
{
  if(vectTaskMouse.size() > 0)
    return vectTaskMouse.back();
  return "";
}
//--------------------------------------------------------------------------------------------

// ������� ������ ������ � ������
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

//--------------------------------------------------------------------------------------------

void myDMapView::drawContents(QPainter* p, int cx, int cy, int cw, int ch)
{

  long int w, h;
  int l = 0, t = 0;

  mapLogic->getMapImageSize(w, h);

  if(w > _cw)
    l = (w - _cw) / 2;
  if(h > _ch)
    t = (h - _ch) / 2;

  QImage _image;
  if(getImageMap(l, t, _cw, _ch, _image))
  {
    //bool saved = _image.save("c:\\image.png");

    currentMap_ = QPixmap::fromImage(_image);

    if(!graphicsItemMap_)
    {
      graphicsItemMap_ = scene_.addPixmap(currentMap_);
      graphicsItemMap_->setPos(QPointF(0,0));
      graphicsItemMap_->setFlags(QGraphicsItem::ItemIsMovable);
    }
    else
      graphicsItemMap_->setPixmap(currentMap_);

    //graphicsItemMap_->setScale(0.1);
    //QTransform transform = graphicsItemMap_->transform();
    //transform.
  }
  return;

  if(cx < 0 || cy < 0 || cw<= 0 || ch <= 0)
    return;
  if(_oldScale!=GetViewScale() || cx != _cx || cy!=_cy || cw!=_cw || ch!=_ch)
  {
    int dx = x() - _contentsX;
    int dy = y() - _contentsY;

    int _x = dx > 0? 0: abs(dx);
    int _y = dy > 0? 0: abs(dy);

    int _x2 = dx > 0? abs(dx): 0;
    int _y2 = dy > 0? abs(dy): 0;

    //    QPainter cur_paint(&_currentPixmapMapViewPort);
    //    QPainter old_paint(&old_currentPixmapMapViewPort);

    //    cur_paint.drawPixmap(_x, _y, _currentPixmapMapViewPort, _x2, _y2,
    //                     _currentPixmapMapViewPort.width() - abs(dx),
    //                     _currentPixmapMapViewPort.height() - abs(dy));

    _cx = cx;
    _cy = cy;
    _cw = cw;
    _ch = ch;
    _oldScale = GetViewScale();
    _contentsX = x();
    _contentsY = y();

    //    QImage _image;
    //    if(getImageMap(0, 0, width, height, _image))
    //    {

    //p->drawPixmap(0, 0, _cw, _ch, _pixmap);

    //      cur_paint.drawPixmap(_cx - x(), _cy - y(), _pixmap, 0, 0, _cw, _ch);
    //      old_paint.drawPixmap(0, 0, _currentPixmapMapViewPort, 0, 0,
    //                           old_currentPixmapMapViewPort.width(),
    //                           old_currentPixmapMapViewPort.height());
    //DrawTask("Map");
    //    }

    //cur_paint.end();
    //old_paint.end();
  }

  //p->drawPixmap(cx, cy, old_currentPixmapMapViewPort, cx - widget()->x(), cy - widget()->y(), cw, ch);

}
//--------------------------------------------------------------------------------------------

void myDMapView :: mousePressEvent(QMouseEvent* e)
{
  int x = e->x();
  int y = e->y();

  double height = getHeight_XY(x, y);

  double B, L;
  XY_BL(x, y, B, L);

  //emit MouseDown(GetActiveMouseTask(), e->x(), e->y(), x, y, B, L, height, e->button(), e->state());

  QGraphicsView::mousePressEvent(e);
}

//--------------------------------------------------------------------------------------------

void myDMapView :: mouseMoveEvent(QMouseEvent* e)
{
  int x = e->x();
  int y = e->y();

  double height = getHeight_XY(x, y);

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

  double height = getHeight_XY(x, y);

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

void myDMapView::refreshSize()
{
  mapLogic->getMapImageSize(_cw, _ch);

  viewport()->resize(_cw, _ch);
}


void myDMapView :: resizeEvent(QResizeEvent* e)
{
  //Q3ScrollView :: viewportResizeEvent(e);
  //_currentPixmapMapViewPort.resize(viewport()->width(), viewport()->height());
  //old_currentPixmapMapViewPort.resize(viewport()->width(), viewport()->height());
  //task_currentPixmapMapViewPort.resize(viewport()->width(), viewport()->height());

  QGraphicsView::resizeEvent(e);
}

void myDMapView::wheelEvent(QWheelEvent *e)
{
  if(e->modifiers().testFlag(Qt::ControlModifier))
  {
    QPoint numPixels = e->pixelDelta();
    QPoint numDegrees = e->angleDelta() / 8;

    if(!numPixels.isNull())
    {
      e->accept();
      //qDebug() << "wheel1 x: " << numPixels.x() << " wheel1 y: " << numPixels.y();
      return;
    }
    else if(!numDegrees.isNull())
    {
      e->accept();
      QPoint numSteps = numDegrees / 15;
      if(numSteps.y() > 0)
        scale(1.1, 1.1);
      else
        scale(1/1.1, 1/1.1);
      //qDebug() << "wheel2 x: " << numSteps.x() << " wheel2 y: " << numSteps.y();
      return;
    }
  }

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
  //QPainter p(this);
  //drawContents(&p, x(), y(), width(), height());

  QGraphicsView::paintEvent(pe);
}
