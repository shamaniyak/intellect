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
#include <QGraphicsSimpleTextItem>
#include <QtDebug>
#include <Src/cartographymap.h>

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

MapView :: MapView(QWidget *parent) : QGraphicsView(parent), map_(NULL)
{
  viewport()->setMouseTracking(true);
  //viewport()->setAttribute(Qt::WA_AcceptTouchEvents);

  scene_.setItemIndexMethod(QGraphicsScene::NoIndex);
  setScene(&scene_);

  graphicsItemMap_ = new MapGraphicsPixmapItem();
  scene_.addItem(graphicsItemMap_);
  graphicsItemMap_->setPos(QPointF(0,0));
  //graphicsItemMap_->setAcceptTouchEvents(true);

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

MapView :: ~MapView()
{
  qDebug() << "myDMapView :: ~myDMapView()";
  map<QString, TBaseClassDraw* > :: iterator it = mapTaskDraw.begin();
  vector<QString> vectNameTask;
  for(; it != mapTaskDraw.end();it++)
    vectNameTask.push_back(it->first);
  for(unsigned int i = 0; i < vectNameTask.size(); i++)
    DeleteTaskDraw(vectNameTask[i]);
}
//--------------------------------------------------------------------------------------------

void MapView :: setMapLogic(IMap* map)
{
  map_ = map;

  graphicsItemMap_->setMap(map_);
}

void MapView::setMap(CartographyMap *map)
{
  if(map)
  {
    setMapLogic(map->map());
    connect(map, &CartographyMap::changed, this, &MapView::on_change);
  }
}

// ��������� ������ � �����


// ��������� ������ � ����� �� �������� �����������
bool MapView :: getHeight_XY(double x, double y, double &h)
{
  if (GetMapScale() == MAXSCALE_40M)
    return false;

  h = map_->getHeight_XY(x, y);
  return true;
}

// ��������� ���������� ���������
//���������� ��� ������:
//0 - ��� ������, -1 - ��� ���������������� �����, -2 - ������ �������� ��������� ���������
int MapView :: getMagneticAngle_BL(double B, double L, double &Ma)
{
  if (GetMapScale() == MAXSCALE_40M)
    return -1;

  return getMagneticAngle_BL(B, L, &Ma)? 0 : -2;
}
//--------------------------------------------------------------------------------------------

// ��������� ���������� ���������
bool MapView :: getMagneticAngle_BL(double B, double L, double* Ma)
{
  assert(map_ != NULL && "getMagneticAngle_BL");

  return map_->getMagneticAngle_BL(B, L, Ma);
}
//--------------------------------------------------------------------------------------------

bool MapView :: isMtrExist()
{
  assert(map_ != NULL && "isMtrExist");

  return map_->isMtrExist();
}

//--------------------------------------------------------------------------------------------

// �������� �������� ������

// ���������� ������� ����� ��� �������� �����
void MapView :: setMapMtr(QString mtrName)
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

// ������� ����� ����
void MapView :: openWorld()
{
  assert(map_ != NULL && "openWorld");

  map_->openWorld();

  //��������� �������� �����������
  //refreshSize();
  showMap();

#if OS_VERSION == 0 // @TODO@ ��������, ������� ���������
  //SetMapLeftTop(1600, 200); // ������
#else
  //SetMapLeftTop(1900, 500); // ����
#endif
}

//--------------------------------------------------------------------------------------------

bool MapView :: isWorldMapOpened()
{
  return map_->getMapFileName().contains("world");
}

// ��������� ���������� ����������� �����

// ���������� ������� ������� ����� (-16  16)
void MapView :: SetMapBright(long int value)
{
  assert(map_ != NULL && "SetMapBright");

  map_->setMapBright(value);
}
//--------------------------------------------------------------------------------------------

// �������� ������� �������
long int MapView :: GetMapBright()
{
  assert(map_ !=NULL && "GetMapBright");

  return map_->getMapBright();
}
//--------------------------------------------------------------------------------------------

// ���������� ������� ��������� �����������
void MapView :: SetMapContrast(long int value)
{
  assert(map_ != NULL && "SetMapContrast");

  map_->setMapContrast(value);
}
//--------------------------------------------------------------------------------------------

// �������� ������� ������� ���������
long int MapView :: GetMapContrast()
{
  assert(map_ != NULL && "GetMapContrast");

  return map_->getMapContrast();
}
//--------------------------------------------------------------------------------------------

// ���������� ����������� ����������� (true - ������ ��������� �����)
void MapView :: SetMapContour(bool value)
{
  assert(map_ != NULL && "SetMapContour");

  map_->setMapContour(value);
}
//--------------------------------------------------------------------------------------------

// �������� ������� ��������� ����� (true - ������ ���������)
bool MapView :: GetMapContour()
{
  assert(map_ != NULL && "GetMapContour");

  return map_->getMapContour();
}
//--------------------------------------------------------------------------------------------

// ���������� ������� �����������
void MapView :: SetViewScale(float value)
{

}
//--------------------------------------------------------------------------------------------

// �������� ������� �����������
int MapView :: GetViewScale()
{
  assert(map_ != NULL && "GetViewScale");

  return map_->getViewScale();
}


void MapView :: GetViewScaleForObjectShaper(int* scale)
{
  *scale = GetViewScale();
}

void MapView::on_change()
{
  showMap();
}
//--------------------------------------------------------------------------------------------

// �������� ������� �����
int MapView :: GetMapScale()
{
  assert(map_ != NULL && "GetMapScale");

  return map_->getMapScale();
}
//--------------------------------------------------------------------------------------------
//����������� ����������� �� ������� ����� � ��� ����
void MapView :: ZoomMapIn()
{
  graphicsItemMap_->zoomInOut(graphicsItemMap_->center(), 1);
}

//--------------------------------------------------------------------------------------------
//��������� ����������� �� ������� ����� � ��� ����
void MapView :: ZoomMapOut()
{
  graphicsItemMap_->zoomInOut(graphicsItemMap_->center(), -1);
}

void MapView::SetCenter(double b, double l)
{
  double x, y;
  map_->BL_XY(b, l, x, y);
  graphicsItemMap_->setCenter(QPointF(x, y));
}

//--------------------------------------------------------------------------------------------
//�������� ������� ����������� ����������
double MapView :: GetMapZoom()
{
  double ms = map_->getMapScale();
  double vs = map_->getViewScale();
  return(ms / vs);
}

// ���������� ������������� ���������� ������ �����
void MapView :: calcCurrentCentreRegion(double &B, double &L)
{
  assert(map_ != NULL && "calcCurrentCentreRegion");

  map_->calcCurrentCentreRegion(B, L);
}
//--------------------------------------------------------------------------------------------

// ��������� � ��������� ��������� ����� �� ����� (�������, ��������, ������ � ����)
void MapView :: readSettings(bool restoreSettings)
{
  assert(map_ != NULL && "readSettings");
  map_->readSettings(restoreSettings);
  // InvalidateTask("cartfunc"); //??? � ������ ����������� ������ 2012, 2027
}
//--------------------------------------------------------------------------------------------

// �������� ��������� ����� � ���� (�������, ��������, ������ � ����)
void MapView :: writeSettings()
{
  assert(map_ != NULL && "writeSettings");

  map_->writeSettings();
}
//--------------------------------------------------------------------------------------------

// �������� ��������� ���������� ����� � ���� (�������, ��������, ������ � ����)
void MapView :: writePreviousSettings()
{
  assert(map_ != NULL && "writePreviousSettings");

  map_->writePreviousSettings();
}
//--------------------------------------------------------------------------------------------

// ���������� ��������� ���� �� ������� (����� ��������� ���������� ��������� ��������� setViewSelect() )
void MapView :: selectLayer(int index, bool selected)
{
  assert(map_ != NULL && "selectLayer");

  map_->selectLayer(index, selected);
}
//--------------------------------------------------------------------------------------------
void MapView :: selectObject(long inCode, bool selected)
{
  assert(map_ != NULL && "selectObject");

  map_->selectObject(inCode, selected);
}
//--------------------------------------------------------------------------------------------

// �������� ������� ��������� ���� �� �������
bool MapView :: isLayerSelected(int index)
{
  assert(map_ != NULL && "isLayerSelected");

  return map_->isLayerSelected(index);
}
//--------------------------------------------------------------------------------------------
bool MapView :: isObjectSelected(long inCode)
{
  assert(map_ != NULL && "isObjectSelected");

  return map_->isObjectSelected(inCode);
}
//--------------------------------------------------------------------------------------------

// �������� ���������� ���� �����
int MapView :: getLayersCount()
{
  assert(map_ != NULL && "getLayersCount");

  return map_->getLayersCount();
}
//--------------------------------------------------------------------------------------------

// �������� ���������� �������� �����
int MapView :: getObjectsCount()
{
  assert(map_ != NULL && "getObjectsCount");

  return map_->getObjectsCount();
}

int MapView :: getObjectsCountInLayer(int layerIndex)
{
  assert(map_ != NULL && "getObjectsCountInLayer");

  return map_->getObjectsCountInLayer(layerIndex);
}

//�������� ��� ������� ���� �� ����������� ������
QString MapView :: getLayerObjectName(int layerIndex, int objectIndex)
{
  assert(map_ != NULL && "getLayerObjectName");

  return map_->getLayerObjectName(layerIndex, objectIndex);
}

//�������� ��� ������� ���� �� ����������� ������
long MapView :: getLayerObjectCode(int layerIndex, int objectIndex)
{
  assert(map_ != NULL && "getLayerObjectCode");

  return map_->getLayerObjectCode(layerIndex, objectIndex);
}

//�������� ������ ���� �� ������ �������
int MapView :: getLayerIndex(long inCode)
{
  assert(map_ != NULL && "getLayerIndex");

  return map_->getLayerIndex(inCode);
}

//�������� ���������� ������ ���� �������� ����
QList<long> MapView :: getLayerObjectsCodes(int layerIndex)
{
  assert(map_ != NULL && "getLayerObjectsCodes");

  return map_->getLayerObjectsCodes(layerIndex);
}

// �������� ��� ���� �� �������
QString MapView :: getMapLayerName(int index)
{
  assert(map_ != NULL && "getMapLayerName");

  return map_->getMapLayerName(index);
}
//--------------------------------------------------------------------------------------------
QString MapView :: getMapObjectName(int index)
{
  assert(map_ != NULL && "getMapObjectName");

  return map_->getMapObjectName(index);
}
//--------------------------------------------------------------------------------------------

// �������� ��� �������� ���� �� ������� ������� �����
QString MapView :: getMapVisibleLayerName(int index)
{
  assert(map_ != NULL && "getMapVisibleLayerName");

  return map_->getMapVisibleLayerName(index);
}
//--------------------------------------------------------------------------------------------
QString MapView :: getMapVisibleObjectName(int index)
{
  assert(map_ != NULL && "getMapVisibleObjectName");

  return map_->getMapVisibleObjectName(index);
}
//--------------------------------------------------------------------------------------------

// �������� ���������� ������� �����
int MapView :: getVisibleLayersCount()
{
  assert(map_ != NULL && "getVisibleLayersCount");

  return map_->getVisibleLayersCount();
}
//--------------------------------------------------------------------------------------------
int MapView :: getVisibleObjectsCount()
{
  assert(map_ != NULL && "getVisibleObjectsCount");

  return map_->getVisibleObjectsCount();
}
//--------------------------------------------------------------------------------------------

// ��������� ��������� ��������� �� ������ � �������� �����
void MapView :: setViewSelect()
{
  assert(map_ != NULL && "setViewSelect");

  map_->setViewSelect();
  Repaint();
}
//--------------------------------------------------------------------------------------------


// ���������

// �������� ����������� �����
long int MapView :: getImageMap(int left, int top, int width, int height, QImage& image)
{
  assert(map_ != NULL && "getImageMap");
  return map_->getImageMap(left, top, width, height, image);
}
//--------------------------------------------------------------------------------------------

QPixmap MapView :: getCurrentPixmap()
{
  return isTaskPixmapFilled ? task_currentPixmapMapViewPort:old_currentPixmapMapViewPort;
}

//--------------------------------------------------------------------------------------------

// ������������ �������
void MapView :: Repaint()
{

}

void MapView::showMap()
{

}

// ���������� ������ ��� ���������
void MapView :: SetDrawTask(QString nameTask, TDrawData _draw, vector<QString> ListTasks)
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
void MapView :: DrawTask(QString nameTask)
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

void MapView::DrawTask(QString nameTask, set<QString >& _drawTask, QPainter* p)
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

QPointF MapView::getMapMousePos() const
{
  return mapMousePos_;
}

void MapView::showTextHint()
{
  auto p = mapMousePos_;
  double b = 0, l = 0, h = 0;
  if(map_) {
    map_->XY_BL(p.x(), p.y(), b, l);
    if( getHeight_XY(p.x(), p.y(), h) )
      ;
  }

  QString text = "Map B: %1, L: %2, H: %3";
  text = text.arg(b).arg(l).arg(h);
  textItem_->setPos(0, 0);
  textItem_->setText(text);
}
//--------------------------------------------------------------------------------------------

// ��������� ������ ��������� (� ��������� � ���)
void MapView :: InvalidateTask(QString nameTask)
{
  //DrawTask(nameTask);
  //viewport()->repaint(false);
}
//--------------------------------------------------------------------------------------------

// ������� ������ ���������
void MapView :: DeleteTaskDraw(QString nameTask)
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
void MapView :: SetActiveMouseTask(QString value)
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
QString MapView :: GetActiveMouseTask(void)
{
  if(vectTaskMouse.size() > 0)
    return vectTaskMouse.back();
  return "";
}
//--------------------------------------------------------------------------------------------

// ������� ������ ������ � ������
void MapView :: DeleteMouseTask(QString value)
{
  vector<QString >::iterator it = std::find(vectTaskMouse.begin(), vectTaskMouse.end(), value);
  if(it != vectTaskMouse.end())
    vectTaskMouse.erase(it);
}
//--------------------------------------------------------------------------------------------

bool MapView :: isMouseTaskExist(QString value)
{
  vector<QString >::iterator it = std::find(vectTaskMouse.begin(), vectTaskMouse.end(), value);
  return (it != vectTaskMouse.end());
}

void MapView :: mousePressEvent(QMouseEvent* e)
{
  int x = mapMousePos_.x();
  int y = mapMousePos_.y();
  double B, L, H = 0;

  map_->XY_BL(x, y, B, L);
  getHeight_XY(x, y, H);

  emit MouseDown(GetActiveMouseTask(), e->x(), e->y(), x, y, B, L, H, e->button(), e->button());

  QGraphicsView::mousePressEvent(e);
}

//--------------------------------------------------------------------------------------------

void MapView :: mouseMoveEvent(QMouseEvent* e)
{
  auto imageLT = graphicsItemMap_->imageLT();
  mousePos_ = e->pos();
  mapMousePos_ = imageLT + mousePos_;

  showTextHint();

  int x = mapMousePos_.x();
  int y = mapMousePos_.y();
  double B, L, H = 0;

  map_->XY_BL(x, y, B, L);
  getHeight_XY(x, y, H);

  emit MouseMove(GetActiveMouseTask(), e->x(), e->y(), x, y, B, L, H, e->button(), e->button());

  QGraphicsView::mouseMoveEvent(e);
}
//--------------------------------------------------------------------------------------------

void MapView :: mouseReleaseEvent(QMouseEvent* e)
{
  int x = mapMousePos_.x();
  int y = mapMousePos_.y();
  double B, L, H;

  map_->XY_BL(x, y, B, L);
  getHeight_XY(x, y, H);

  emit MouseUp(GetActiveMouseTask(), e->x(), e->y(), x, y, B, L, H, e->button(), e->button());

  QGraphicsView::mouseReleaseEvent(e);
}
//--------------------------------------------------------------------------------------------

void MapView :: keyPressEvent(QKeyEvent * keyEvent)
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

      auto mapCentre = graphicsItemMap_->center();
      int x = mapCentre.x(), y = mapCentre.y();
      double B, L, height;
      map_->XY_BL(x, y, B, L);
      getHeight_XY(x, y, height);

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

bool MapView::viewportEvent(QEvent *e)
{
//  QString str;
//  switch(e->type())
//  {
//    case QEvent::TouchBegin:
//      str = "Touch Begin";
//      break;
//    case QEvent::TouchEnd:
//      str = "Touch End";
//      break;
//    case QEvent::TouchCancel:
//      str = "Touch Cancel";
//      break;
//    case QEvent::TouchUpdate:
//      str = "Touch Update";
//      break;
//  }

//  QTouchEvent *te = dynamic_cast<QTouchEvent*>(e);
//  if(te)
//  {
//    str += "\n";
//    auto points = te->touchPoints();
//    int i = 0;
//    for(auto p: points)
//    {
//      ++i;
//      auto pos1 = p.pos();
//      QString str1 = "p %1: x=%2, y=%3; ";
//      str1 = str1.arg(i).arg(pos1.x()).arg(pos1.y());
//      str += str1;
//    }
//  }

//  if(textItem_ && !str.isEmpty())
//    textItem_->setText(str);

  QGraphicsView::viewportEvent(e);
}

void MapView :: resizeEvent(QResizeEvent* e)
{
  scene()->setSceneRect(0, 0, viewport()->width(), viewport()->height());
  graphicsItemMap_->resize(viewport()->width(), viewport()->height());

  QGraphicsView::resizeEvent(e);
}

void MapView::wheelEvent(QWheelEvent *e)
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


void MapView :: grabMapViewKeyboard()
{
  grabKeyboard();
  keyboardGrabbed = true;
}

void MapView :: releaseMapViewKeyboard()
{
  releaseKeyboard();
  keyboardGrabbed = false;
}

bool MapView :: isKeyboardGrabbed()
{
  return keyboardGrabbed;
}

void MapView::paintEvent(QPaintEvent *pe)
{

  QGraphicsView::paintEvent(pe);
}
