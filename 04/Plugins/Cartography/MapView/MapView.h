//	���� ���� �������� ������ ������� �� "�����������".
//	����� ��������: ��� ��� 7 13:14:38 2007

#ifndef MapViewH
#define MapViewH

#include <QCoreApplication>
#include <qpainter.h>
#include <QGraphicsView>
#include <QMouseEvent>
#include <set>
#include "MapGraphicsPixmapItem.h"

using namespace std;

class IMap;
class CartographyMap;
class QGraphicsSimpleTextItem;

//������� ���������� ������������ ��������� ��������� � ������ ��������.
//����������� ��� 1.000.000 - 4.500 ��.
int GetMaxDistanceToDraw(int scale);

//--------------------------------------------------------------------------------------------

class TBaseClassDraw
{
    Q_DECLARE_TR_FUNCTIONS(TBaseClassDraw)
public:
  virtual ~TBaseClassDraw() = 0;
    virtual void Draw(QPainter* p) = 0;
};
//--------------------------------------------------------------------------------------------

template<class TParrentClassDraw> class TClassDraw : public TBaseClassDraw
{
    Q_DECLARE_TR_FUNCTIONS(TParrentClassDraw)
    friend class myDMapView;
private:
    TParrentClassDraw* ClassDraw;
    void (TParrentClassDraw :: *voidClassTask)(QPainter* );
public:
    TClassDraw(TParrentClassDraw* _ClassDraw, void (TParrentClassDraw :: *_voidClassTask)(QPainter* ))
    {
        ClassDraw = _ClassDraw;
        voidClassTask = _voidClassTask;
    }
    void Draw(QPainter* p)
    {
        (ClassDraw->*voidClassTask)(p);
    }
};
//--------------------------------------------------------------------------------------------

class TDrawData
{
    Q_DECLARE_TR_FUNCTIONS(TDrawData)
    friend class myDMapView;

private:
    TBaseClassDraw* _dataDraw;

public:
    TDrawData()
    {
        _dataDraw = NULL;
    }
    template<class TDrawCl> void SetData(TDrawCl* _ptrCl, void (TDrawCl :: *voidDraw)(QPainter* ))
    {
        if(_dataDraw)
            delete _dataDraw;
        _dataDraw = new TClassDraw<TDrawCl>(_ptrCl, voidDraw);
    }
    template<class TDrawCl> static TDrawData Data(TDrawCl* _ptrCl, void (TDrawCl :: *voidDraw)(QPainter* ))
    {
        TDrawData _returnData;
        if(_returnData._dataDraw)
            delete _returnData._dataDraw;
        _returnData._dataDraw = new TClassDraw<TDrawCl>(_ptrCl, voidDraw);
        return _returnData;
    }
};
//--------------------------------------------------------------------------------------------

// ����� ��� ����������� � ��������� ������ � �����
class myDMapView : public QGraphicsView
{
  Q_OBJECT

public:

    explicit myDMapView(QWidget *parent = 0);
    ~myDMapView();


    void setMapLogic(IMap* map);
    void setMap(CartographyMap* map);

    // ������� ���������

    // ������� ��������� �� ������������� � ������� �������� �����
    void BL_XY(double B, double L, double& X, double& Y);
    // ������� ��������� �� �������� �������� ����� � �������������
    void XY_BL(double X, double Y, double& B, double& L);
    // ������� ������������� ���������  � ���������� ������������� ������� �������
    void BL_XmYm(double B, double L, double& Xm, double& Ym);
    // ������� ��������� �� ������� ������������� ������� � �������������
    void XmYm_BL(double Xm, double Ym, double& B, double& L);
    // ������� ��������� �� �������� �������� � ���������� ������� ������������� �������
    void XY_XmYm(double X, double Y, double& Xm, double& Ym);
    // ������� ��������� �� ������� ������������� ������� � ������� ��������
    void XmYm_XY(double Xm, double Ym, double& X, double& Y);

    // ��������� ������ � �����

    // ��������� ������ � ����� �� �������� �����������
    double getHeight_XY(double x, double y);
    bool getHeight_XY(double x, double y, double &h);

    // ��������� ������ � ����� �� ������ � �������
    double getHeight_BL(double B, double L);
    void getHeight_BL(double B, double L, double* H);
    bool getHeight_BL(double B, double L, double &H);

    // ��������� ������ � ����� �� ������������� �����������
    double getHeight_XmYm(double Xm, double Ym);

    // ��������� ���������� ���������
    bool getMagneticAngle_BL(double B, double L, double* Ma);
    int getMagneticAngle_BL(double B, double L, double &Ma);

    // �������� �� ����� ������� �����
    bool isMtrExist();

    // �������� �������� ������

    // ���������� ������� ����� ��� �������� �����
    void setMapMtr(QString mtrName);
    // ������� ����� ����
    void openWorld();
    // ������� �� ������� ������ ����� ����
    bool isWorldMapOpened();


    // ��������� ���������� ����������� �����

    // �������� ����� ������������ ����� � ������������� �����������
    void getMapCenter(double& centerB, double& centerL);
    // ���������� ����� ����� � ����� ������
    void setMapCenterToScreenCenter();
    // ���������� ������������� ���������� ������ �����
    void calcCurrentCentreRegion(double &B, double &L);
    // �������� ����� ��������� ����������� � ������������� �����������
    void getCenterOpenRegion(double &_CenterB, double &_CenterL);
    // �������� ������� �������� ���������� ������ �������� ����
    void GetMapLeftTop(int * left, int * top);
    //�������� ���������� ������ �������� ���� ������
    void getMapLeftTopBL(double& B, double& L);
    //�������� ���������� ������� ������� ���� ������
    void getMapRightBottomBL(double& B, double& L);
    //���������� ����� ������� ���� ������ � ��������� ���������� B L
    void setMapLeftTopBL(double B, double L);
    // ��������� � ��������� ��������� ����� �� ����� (�������, ��������, ������ � ����)
    void readSettings(bool restoreSettings = false);
    // �������� ��������� ����� � ���� (�������, ��������, ������ � ����)
    void writeSettings();
    // �������� ��������� ���������� ����� � ���� (�������, ��������, ������ � ����)
    void writePreviousSettings();
    // ���������� ��������� ���� �� ������� (����� ��������� ���������� ��������� ��������� setViewSelect() )
    void selectLayer(int index, bool selected);
    // ���������� ��������� ������� �� ������� (����� ��������� ���������� ��������� ��������� setViewSelect() )
    void selectObject(long inCode, bool selected);
    // �������� ������� ��������� ���� �� �������
    bool isLayerSelected(int index);
    // �������� ������� ��������� ������� �� ����������� ����
    bool isObjectSelected(long inCode);
    // �������� ��� ���� �� �������
    QString getMapLayerName(int index);
    // �������� ��� ������� �� �������
    QString getMapObjectName(int index);
    // �������� ��� �������� ���� �� ������� ������� �����
    QString getMapVisibleLayerName(int index);
    // �������� ��� �������� ������� �� ������� ������� �����
    QString getMapVisibleObjectName(int index);
    // �������� ���������� ���� �����
    int getLayersCount();
    // �������� ���������� �������� �����
    int getObjectsCount();
    // �������� ���������� ������� �����
    int getVisibleLayersCount();
    // �������� ���������� ������� ��������
    int getVisibleObjectsCount();
    // ��������� ��������� ��������� �� ������ � �������� �����
    void setViewSelect();
    // �������� ���������� �������� ����� � ����
    int getObjectsCountInLayer(int layerIndex);
    //�������� ��� ������� ���� �� ����������� ������
    QString getLayerObjectName(int layerIndex, int objectIndex);
    //�������� ��� ������� ���� �� ����������� ������
    long getLayerObjectCode(int layerIndex, int objectIndex);
    //�������� ������ ���� �� ������ �������
    int getLayerIndex(long inCode);
    //�������� ���������� ������ ���� �������� ����
    QList<long> getLayerObjectsCodes(int layerIndex);

    // ���������

    // �������� ����������� �����
    long int getImageMap(int left, int top, int width, int height, QImage& image);

    // �������� ������� �����������
    QPixmap getCurrentPixmap();

    // ������������ �������
    void Repaint();

    // ���������� ������ ��� ���������, ���������:
    // 1 - ��� ������ (�������������)
    // 2 - ����� ���������� ��������� �� ������� ���������
    // 3 - ������ ����� ������� ���������� ������������ ��� ��������� ��������� ��������� ������
    void SetDrawTask(QString ,TDrawData , vector<QString>);

    // ��������� ������ ��������� (� ��������� � ���)
    void DrawTask(QString);
    void InvalidateTask(QString);
    // ������� ������ ���������
    void DeleteTaskDraw(QString);

    // ������ � ������

    // �������� ��� ������� ������� ������ ������ � ������
    void SetActiveMouseTask(QString);
    // �������� ������� ������ ����� � ������
    QString GetActiveMouseTask(void);
    // ������� ������ ������ � ������
    void DeleteMouseTask(QString);
    // ���������� �� ������ ����
    bool isMouseTaskExist(QString);

    // ������ � �����������
    void grabMapViewKeyboard();
    void releaseMapViewKeyboard();
    bool isKeyboardGrabbed();

public slots:
    // �������� �����
    void showMap();
    // ������� ��������� �� ������������� � ������� �������� �����
    void BL_XY(double B, double L, int* X, int* Y);
    // ������� ��������� �� �������� �������� ����� � �������������
    void XY_BL(int X, int Y, double* B, double* L);
    // ��������� ��������
    void GetViewScaleForObjectShaper(int*);

    // ��������� ���������� ����������� �����

    // ���������� ������� ������� ����� (-16  16)
    void SetMapBright(long int value);
    // �������� ������� �������
    long int GetMapBright();
    // ���������� ������� ��������� �����������
    void SetMapContrast(long int value);
    // �������� ������� ������� ���������
    long int GetMapContrast();
    // ���������� ����������� ����������� (true - ������ ��������� �����)
    void SetMapContour(bool value);
    // �������� ������� ��������� ����� (true - ������ ���������)
    bool GetMapContour();
    // ���������� ������� �����������
    void SetViewScale(float value);
    // �������� ������� �����������
    int GetViewScale();
    // �������� ������� �����
    int GetMapScale();
    //�������� ������� ����������� ����������
    double GetMapZoom();
    //����������� ����������� �� ������� ����� � ��� ����
    void ZoomMapIn(double B = 0, double L = 0);
    //��������� ����������� �� ������� ����� � ��� ����
    void ZoomMapOut(double B = 0, double L = 0);
    // ���������� ������������� ���������� ������ ������������ �����
    void setMapCenter(double centerB, double centerL);

signals:

    void MouseDown(QString mouseTask, int x, int y, int X, int Y, double B, double L, double H, Qt::MouseButton btn, Qt::MouseButton btnState);
    void MouseMove(QString mouseTask, int x, int y, int X, int Y, double B, double L, double H, Qt::MouseButton btn, Qt::MouseButton btnState);
    void MouseUp(QString mouseTask, int x, int y, int X, int Y, double B, double L, double H, Qt::MouseButton btn, Qt::MouseButton btnState);
    void escPressedSignal();

protected slots:
    void on_change();

protected:

    virtual void paintEvent(QPaintEvent *pe);
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void resizeEvent(QResizeEvent* e);

    virtual void wheelEvent(QWheelEvent *e);

    virtual void keyPressEvent(QKeyEvent * keyEvent);

private:

    // ���������� ������� �������� ���������� ������ �������� ����
    void SetMapLeftTop(int left, int top);

    // �������� ������� �������� ���������� ������� ������� ����
    void GetMapRightBottom(int * right, int * bottom);

    // ��������� ������ ��������� (� ��������� � ���
    void DrawTask(QString ,  set<QString >&, QPainter* );

    // ������ ������ � ������
    IMap* map_;
    // ����� ��� ���������
    QGraphicsScene scene_;
    // ������� ����� "����������� �����"
    MapGraphicsPixmapItem *graphicsItemMap_ = 0;
    // ������� ��� ���������
    QGraphicsSimpleTextItem *textItem_ = nullptr;

    QPixmap old_currentPixmapMapViewPort;
    QPixmap task_currentPixmapMapViewPort;

    QPointF mousePos_;

    map<QString, TBaseClassDraw* > mapTaskDraw;
    map<QString, TBaseClassDraw* >::iterator it_activeDrawTask;
    map<QString, vector<QString> > mapListTaskInvalidateForTask;
    vector<QString > vectTaskMouse;

    bool keyboardGrabbed;
    bool isTaskPixmapFilled;

    void showTextHint();
};
//--------------------------------------------------------------------------------------------

#endif
