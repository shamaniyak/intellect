//	Этот файл является частью проекта ПС "Конструктор".
//	Время создания: Пнд Июн 10 14:22:51 2013
#ifndef IMapH
#define IMapH

#include <QCoreApplication>
#include <QImage>

// Интерфейс работы с картой
// Используется для поключения к myDMapView различных реализаций функций работы с картой
class IMap
{

public:
  virtual void release() = 0;

    // ПЕРЕВОД КООРДИНАТ

    // Перевод геодезических координат  в координаты прямоугольной плоской системы
    virtual void BL_XmYm(double B, double L, double& Xm, double& Ym) = 0;
    // Перевод координат из плоской прямоугольной системы в геодезические
    virtual void XmYm_BL(double Xm, double Ym, double& B, double& L) = 0;
    // Перевод координат из пикселей картинки в координаты плоской прямоугольной системы
    virtual void XY_XmYm(double X, double Y, double& Xm, double& Ym) = 0;
    // Перевод координат из плоской прямоугольной системы в пиксели картинки
    virtual void XmYm_XY(double Xm, double Ym, double& X, double& Y) = 0;
    // Перевод координат из геодезических в пиксели картинки карты
    virtual void BL_XY(double B, double L, double& X, double& Y) = 0;
    // Перевод координат из пикселей картинки карты в геодезические
    virtual void XY_BL(double X, double Y, double& B, double& L) = 0;

    // ПОЛУЧЕНИЕ ДАННЫХ С КАРТЫ

    // Получение высоты в точке по прямоугольным координатам
    virtual double getHeight_XmYm(double Xm, double Ym) = 0;
    virtual double getHeight_XY(double x, double y) = 0;

    // Получение магнитного склонения по геодезическим координатам
    virtual bool getMagneticAngle_BL(double B, double L, double* Ma) = 0;

    // Содержит ли карта матрицу высот
    virtual bool isMtrExist() = 0;

    // ЗАГРУЗКА ИСХОДНЫХ ДАННЫХ

    // Открыть карту по имени файла
    virtual bool setMap(QString mapName) = 0;
    // Получить имя открытой карты
    virtual QString getMapFileName() = 0;
    // Установить матрицу высот для открытой карты
    virtual void setMapMtr(QString mtrName) = 0;
    // Открыть карту мира
    virtual void openWorld() = 0;

    // НАСТРОЙКА ПАРАМЕТРОВ ОТОБРАЖЕНИЯ КАРТЫ

    // Установить уровень яркости карты (-16  16)
    virtual void setMapBright(long int value) = 0;
    // Получить уровень яркости
    virtual long int getMapBright() = 0;
    // Установить уровень контраста изображения
    virtual void setMapContrast(long int value) = 0;
    // Получить текущий уровень контраста
    virtual long int getMapContrast() = 0;
    // Установить контурность изображения (true - значит контурная карта)
    virtual void setMapContour(bool value) = 0;
    // Получить признак контурной карты (true - значит контурная)
    virtual bool getMapContour() = 0;
    // Установить масштаб отображения
    virtual void setViewScale(long int& centerX, long int& centerY, float value) = 0;
    // Получить масштаб отображения
    virtual int getViewScale() = 0;
    // Получить масштаб карты
    virtual int getMapScale() = 0;
    // Установить геодезические координаты центра отображаемой карты
    virtual void setMapCenter(double B, double L) = 0;
    // Получить геодезические координаты центра карты
    virtual void getMapCentre(double &B, double &L) = 0;
    // Рассчитать геодезические координаты центра карты
    virtual void calcCurrentCentreRegion(double &B, double &L) = 0;
    // Прочитать и применить настройки карты из файла (яркость, контраст, контур и слои)
    virtual void readSettings(bool restoreSettings = false) = 0;
    // Записать настройки карты в файл (яркость, контраст, контур и слои)
    virtual void writeSettings(bool restoreSettings = false) = 0;
    // Записать настройки предыдущей карты в файл (яркость, контраст, контур и слои)
    virtual void writePreviousSettings() = 0;
    // Установить видимость слоя по индексу (после установок необходимо применить изменения setViewSelect() )
    virtual void selectLayer(int index, bool selected, bool noCheck = false) = 0;
    // Установить видимость объект по индексу (после установок необходимо применить изменения setViewSelect() )
    virtual void selectObject(long inCode, bool selected, bool noCheck = false) = 0;
     // Получить признак видимости слоя по индексу
    virtual bool isLayerSelected(int index) = 0;
    // Получить признак видимости объекта по внутреннему коду
    virtual bool isObjectSelected(long inCode) = 0;
    // Получить имя слоя по индексу
    virtual QString getMapLayerName(int index) = 0;
    // Получить имя объекта по индексу
    virtual QString getMapObjectName(int index) = 0;
    // Получить имя видимого слоя по индексу видимых слоев
    virtual QString getMapVisibleLayerName(int index) = 0;
    // Получить имя видимого объекта по индексу видимых слоев
    virtual QString getMapVisibleObjectName(int index) = 0;
    // Получить количество всех слоев
    virtual int getLayersCount() = 0;
    // Получить количество видимых слоев
    virtual int getVisibleLayersCount() = 0;
    // Получить количество видимых объектов
    virtual int getVisibleObjectsCount() = 0;
    // Применить изменения связанные со слоями к открытой карте
    virtual void setViewSelect() = 0;
    // Получить количество объектов карты
    virtual int getObjectsCount() = 0;
    // Получить количество объектов карты в слое
    virtual int getObjectsCountInLayer(int layerIndex) = 0;
    //Получить имя объекта слоя по порядковому номеру
    virtual QString getLayerObjectName(int layerIndex, int objectIndex) = 0;
    //Получить код объекта слоя по порядковому номеру
    virtual long getLayerObjectCode(int layerIndex, int objectIndex) = 0;
    //Получить индекс слоя по номеру объекта
    virtual int getLayerIndex(long inCode) = 0;
    //Получить внутренние номера всех объектов слоя
    virtual QList<long> getLayerObjectsCodes(int layerIndex) = 0;

    // ОТРИСОВКА

    // Получить размеры изображения карты
    virtual void getMapImageSize(long int& width, long int& height) = 0;

    // Получить изображение карты
    virtual long int getImageMap(int left, int top, int width, int height, QImage &_image) = 0;

};


#endif
