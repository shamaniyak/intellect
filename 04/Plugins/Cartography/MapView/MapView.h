//	Этот файл является частью проекта ПС "Конструктор".
//	Время создания: Вск Окт 7 13:14:38 2007

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

//Функция возвращает максимальную дистанцию отрисовки с учетом масштаба.
//Максимально для 1.000.000 - 4.500 км.
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

// Класс для отображения и получения данных с карты
class myDMapView : public QGraphicsView
{
  Q_OBJECT

public:

    explicit myDMapView(QWidget *parent = 0);
    ~myDMapView();


    void setMapLogic(IMap* map);
    void setMap(CartographyMap* map);

    // ПЕРЕВОД КООРДИНАТ

    // Перевод координат из геодезических в пиксели картинки карты
    void BL_XY(double B, double L, double& X, double& Y);
    // Перевод координат из пикселей картинки карты в геодезические
    void XY_BL(double X, double Y, double& B, double& L);
    // Перевод геодезических координат  в координаты прямоугольной плоской системы
    void BL_XmYm(double B, double L, double& Xm, double& Ym);
    // Перевод координат из плоской прямоугольной системы в геодезические
    void XmYm_BL(double Xm, double Ym, double& B, double& L);
    // Перевод координат из пикселей картинки в координаты плоской прямоугольной системы
    void XY_XmYm(double X, double Y, double& Xm, double& Ym);
    // Перевод координат из плоской прямоугольной системы в пиксели картинки
    void XmYm_XY(double Xm, double Ym, double& X, double& Y);

    // ПОЛУЧЕНИЕ ДАННЫХ С КАРТЫ

    // Получение высоты в точке по экранным координатам
    double getHeight_XY(double x, double y);
    bool getHeight_XY(double x, double y, double &h);

    // Получение высоты в точке по широте и долготе
    double getHeight_BL(double B, double L);
    void getHeight_BL(double B, double L, double* H);
    bool getHeight_BL(double B, double L, double &H);

    // Получение высоты в точке по прямоугольным координатам
    double getHeight_XmYm(double Xm, double Ym);

    // Получение магнитного склонения
    bool getMagneticAngle_BL(double B, double L, double* Ma);
    int getMagneticAngle_BL(double B, double L, double &Ma);

    // Содержит ли карта матрицу высот
    bool isMtrExist();

    // ЗАГРУЗКА ИСХОДНЫХ ДАННЫХ

    // Установить матрицу высот для открытой карты
    void setMapMtr(QString mtrName);
    // Открыть карту мира
    void openWorld();
    // Открыта ли текущей картой карта мира
    bool isWorldMapOpened();


    // НАСТРОЙКА ПАРАМЕТРОВ ОТОБРАЖЕНИЯ КАРТЫ

    // Получить центр отображаемой карты в геодезических координатах
    void getMapCenter(double& centerB, double& centerL);
    // Установить центр карты в центр экрана
    void setMapCenterToScreenCenter();
    // Рассчитать геодезические координаты центра карты
    void calcCurrentCentreRegion(double &B, double &L);
    // Получить центр открытого изображения в геодезических координатах
    void getCenterOpenRegion(double &_CenterB, double &_CenterL);
    // Получить текущие экранные координаты левого верхнего угла
    void GetMapLeftTop(int * left, int * top);
    //Получить координаты левого верхнего края экрана
    void getMapLeftTopBL(double& B, double& L);
    //Получить координаты нижнего правого края экрана
    void getMapRightBottomBL(double& B, double& L);
    //Установить левый верхний край экрана в указанные координаты B L
    void setMapLeftTopBL(double B, double L);
    // Прочитать и применить настройки карты из файла (яркость, контраст, контур и слои)
    void readSettings(bool restoreSettings = false);
    // Записать настройки карты в файл (яркость, контраст, контур и слои)
    void writeSettings();
    // Записать настройки предыдущей карты в файл (яркость, контраст, контур и слои)
    void writePreviousSettings();
    // Установить видимость слоя по индексу (после установок необходимо применить изменения setViewSelect() )
    void selectLayer(int index, bool selected);
    // Установить видимость объекта по индексу (после установок необходимо применить изменения setViewSelect() )
    void selectObject(long inCode, bool selected);
    // Получить признак видимости слоя по индексу
    bool isLayerSelected(int index);
    // Получить признак видимости объекта по внутреннему коду
    bool isObjectSelected(long inCode);
    // Получить имя слоя по индексу
    QString getMapLayerName(int index);
    // Получить имя объекта по индексу
    QString getMapObjectName(int index);
    // Получить имя видимого слоя по индексу видимых слоев
    QString getMapVisibleLayerName(int index);
    // Получить имя видимого объекта по индексу видимых слоев
    QString getMapVisibleObjectName(int index);
    // Получить количество всех слоев
    int getLayersCount();
    // Получить количество объектов карты
    int getObjectsCount();
    // Получить количество видимых слоев
    int getVisibleLayersCount();
    // Получить количество видимых объектов
    int getVisibleObjectsCount();
    // Применить изменения связанные со слоями к открытой карте
    void setViewSelect();
    // Получить количество объектов карты в слое
    int getObjectsCountInLayer(int layerIndex);
    //Получить имя объекта слоя по порядковому номеру
    QString getLayerObjectName(int layerIndex, int objectIndex);
    //Получить код объекта слоя по порядковому номеру
    long getLayerObjectCode(int layerIndex, int objectIndex);
    //Получить индекс слоя по номеру объекта
    int getLayerIndex(long inCode);
    //Получить внутренние номера всех объектов слоя
    QList<long> getLayerObjectsCodes(int layerIndex);

    // ОТРИСОВКА

    // Получить изображение карты
    long int getImageMap(int left, int top, int width, int height, QImage& image);

    // Получить текущее изображение
    QPixmap getCurrentPixmap();

    // Перерисовать контент
    void Repaint();

    // Установить задачу для отрисовки, параметры:
    // 1 - имя задачи (идентификатор)
    // 2 - класс содержащий указатель на функцию отрисовки
    // 3 - вектор задач которые необходимо перерисовать при изменении отрисовки указанной задачи
    void SetDrawTask(QString ,TDrawData , vector<QString>);

    // Выполнить задачу отрисовки (и связанные с ней)
    void DrawTask(QString);
    void InvalidateTask(QString);
    // удалить задачу отрисовки
    void DeleteTaskDraw(QString);

    // РАБОТА С МЫШКОЙ

    // Добавить или сделать текущей задачу работы с мышкой
    void SetActiveMouseTask(QString);
    // Получить текущую задачу рабты с мышкой
    QString GetActiveMouseTask(void);
    // Удалить задачу работы с мышкой
    void DeleteMouseTask(QString);
    // Существует ли задача мыши
    bool isMouseTaskExist(QString);

    // РАБОТА С КЛАВИАТУРОЙ
    void grabMapViewKeyboard();
    void releaseMapViewKeyboard();
    bool isKeyboardGrabbed();

public slots:
    // Показать карту
    void showMap();
    // Перевод координат из геодезических в пиксели картинки карты
    void BL_XY(double B, double L, int* X, int* Y);
    // Перевод координат из пикселей картинки карты в геодезические
    void XY_BL(int X, int Y, double* B, double* L);
    // Получение масштаба
    void GetViewScaleForObjectShaper(int*);

    // НАСТРОЙКА ПАРАМЕТРОВ ОТОБРАЖЕНИЯ КАРТЫ

    // Установить уровень яркости карты (-16  16)
    void SetMapBright(long int value);
    // Получить уровень яркости
    long int GetMapBright();
    // Установить уровень контраста изображения
    void SetMapContrast(long int value);
    // Получить текущий уровень контраста
    long int GetMapContrast();
    // Установить контурность изображения (true - значит контурная карта)
    void SetMapContour(bool value);
    // Получить признак контурной карты (true - значит контурная)
    bool GetMapContour();
    // Установить масштаб отображения
    void SetViewScale(float value);
    // Получить масштаб отображения
    int GetViewScale();
    // Получить масштаб карты
    int GetMapScale();
    //Получить текущий коэффициент увеличения
    double GetMapZoom();
    //Увеличивает изображение на текущей карте в два раза
    void ZoomMapIn(double B = 0, double L = 0);
    //Уменьшает изображение на текущей карте в два раза
    void ZoomMapOut(double B = 0, double L = 0);
    // Установить геодезические координаты центра отображаемой карты
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

    // Установить текущие экранные координаты левого верхнего угла
    void SetMapLeftTop(int left, int top);

    // Получить текущие экранные координаты правого нижнего угла
    void GetMapRightBottom(int * right, int * bottom);

    // Выполнить задачу отрисовки (и связанные с ней
    void DrawTask(QString ,  set<QString >&, QPainter* );

    // Логика работы с картой
    IMap* map_;
    // Сцена для отрисовки
    QGraphicsScene scene_;
    // Элемент сцены "Изображение карты"
    MapGraphicsPixmapItem *graphicsItemMap_ = 0;
    // Элемент для подсказки
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
