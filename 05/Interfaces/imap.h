//	���� ���� �������� ������ ������� �� "�����������".
//	����� ��������: ��� ��� 10 14:22:51 2013
#ifndef IMapH
#define IMapH

#include <QCoreApplication>
#include <QImage>

// ��������� ������ � ������
// ������������ ��� ���������� � myDMapView ��������� ���������� ������� ������ � ������
class IMap
{

public:
  virtual void release() = 0;

    // ������� ���������

    // ������� ������������� ���������  � ���������� ������������� ������� �������
    virtual void BL_XmYm(double B, double L, double& Xm, double& Ym) = 0;
    // ������� ��������� �� ������� ������������� ������� � �������������
    virtual void XmYm_BL(double Xm, double Ym, double& B, double& L) = 0;
    // ������� ��������� �� �������� �������� � ���������� ������� ������������� �������
    virtual void XY_XmYm(double X, double Y, double& Xm, double& Ym) = 0;
    // ������� ��������� �� ������� ������������� ������� � ������� ��������
    virtual void XmYm_XY(double Xm, double Ym, double& X, double& Y) = 0;
    // ������� ��������� �� ������������� � ������� �������� �����
    virtual void BL_XY(double B, double L, double& X, double& Y) = 0;
    // ������� ��������� �� �������� �������� ����� � �������������
    virtual void XY_BL(double X, double Y, double& B, double& L) = 0;

    // ��������� ������ � �����

    // ��������� ������ � ����� �� ������������� �����������
    virtual double getHeight_XmYm(double Xm, double Ym) = 0;
    virtual double getHeight_XY(double x, double y) = 0;

    // ��������� ���������� ��������� �� ������������� �����������
    virtual bool getMagneticAngle_BL(double B, double L, double* Ma) = 0;

    // �������� �� ����� ������� �����
    virtual bool isMtrExist() = 0;

    // �������� �������� ������

    // ������� ����� �� ����� �����
    virtual bool setMap(QString mapName) = 0;
    // �������� ��� �������� �����
    virtual QString getMapFileName() = 0;
    // ���������� ������� ����� ��� �������� �����
    virtual void setMapMtr(QString mtrName) = 0;
    // ������� ����� ����
    virtual void openWorld() = 0;

    // ��������� ���������� ����������� �����

    // ���������� ������� ������� ����� (-16  16)
    virtual void setMapBright(long int value) = 0;
    // �������� ������� �������
    virtual long int getMapBright() = 0;
    // ���������� ������� ��������� �����������
    virtual void setMapContrast(long int value) = 0;
    // �������� ������� ������� ���������
    virtual long int getMapContrast() = 0;
    // ���������� ����������� ����������� (true - ������ ��������� �����)
    virtual void setMapContour(bool value) = 0;
    // �������� ������� ��������� ����� (true - ������ ���������)
    virtual bool getMapContour() = 0;
    // ���������� ������� �����������
    virtual void setViewScale(long int& centerX, long int& centerY, float value) = 0;
    // �������� ������� �����������
    virtual int getViewScale() = 0;
    // �������� ������� �����
    virtual int getMapScale() = 0;
    // ���������� ������������� ���������� ������ ������������ �����
    virtual void setMapCenter(double B, double L) = 0;
    // �������� ������������� ���������� ������ �����
    virtual void getMapCentre(double &B, double &L) = 0;
    // ���������� ������������� ���������� ������ �����
    virtual void calcCurrentCentreRegion(double &B, double &L) = 0;
    // ��������� � ��������� ��������� ����� �� ����� (�������, ��������, ������ � ����)
    virtual void readSettings(bool restoreSettings = false) = 0;
    // �������� ��������� ����� � ���� (�������, ��������, ������ � ����)
    virtual void writeSettings(bool restoreSettings = false) = 0;
    // �������� ��������� ���������� ����� � ���� (�������, ��������, ������ � ����)
    virtual void writePreviousSettings() = 0;
    // ���������� ��������� ���� �� ������� (����� ��������� ���������� ��������� ��������� setViewSelect() )
    virtual void selectLayer(int index, bool selected, bool noCheck = false) = 0;
    // ���������� ��������� ������ �� ������� (����� ��������� ���������� ��������� ��������� setViewSelect() )
    virtual void selectObject(long inCode, bool selected, bool noCheck = false) = 0;
     // �������� ������� ��������� ���� �� �������
    virtual bool isLayerSelected(int index) = 0;
    // �������� ������� ��������� ������� �� ����������� ����
    virtual bool isObjectSelected(long inCode) = 0;
    // �������� ��� ���� �� �������
    virtual QString getMapLayerName(int index) = 0;
    // �������� ��� ������� �� �������
    virtual QString getMapObjectName(int index) = 0;
    // �������� ��� �������� ���� �� ������� ������� �����
    virtual QString getMapVisibleLayerName(int index) = 0;
    // �������� ��� �������� ������� �� ������� ������� �����
    virtual QString getMapVisibleObjectName(int index) = 0;
    // �������� ���������� ���� �����
    virtual int getLayersCount() = 0;
    // �������� ���������� ������� �����
    virtual int getVisibleLayersCount() = 0;
    // �������� ���������� ������� ��������
    virtual int getVisibleObjectsCount() = 0;
    // ��������� ��������� ��������� �� ������ � �������� �����
    virtual void setViewSelect() = 0;
    // �������� ���������� �������� �����
    virtual int getObjectsCount() = 0;
    // �������� ���������� �������� ����� � ����
    virtual int getObjectsCountInLayer(int layerIndex) = 0;
    //�������� ��� ������� ���� �� ����������� ������
    virtual QString getLayerObjectName(int layerIndex, int objectIndex) = 0;
    //�������� ��� ������� ���� �� ����������� ������
    virtual long getLayerObjectCode(int layerIndex, int objectIndex) = 0;
    //�������� ������ ���� �� ������ �������
    virtual int getLayerIndex(long inCode) = 0;
    //�������� ���������� ������ ���� �������� ����
    virtual QList<long> getLayerObjectsCodes(int layerIndex) = 0;

    // ���������

    // �������� ������� ����������� �����
    virtual void getMapImageSize(long int& width, long int& height) = 0;

    // �������� ����������� �����
    virtual long int getImageMap(int left, int top, int width, int height, QImage &_image) = 0;

};


#endif
