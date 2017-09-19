#ifndef TALGORITHM_H
#define TALGORITHM_H

#include <QObject>
//#include "tmemory.h"
#include "memorymanager.h"
#include "objectmanager.h"
#include "pluginmanager.h"

// ���������� ���������
class TAlgorithm : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QObject* MM READ getMM)
  Q_PROPERTY(QObject* OM READ getOM)
  Q_PROPERTY(QObject* PM READ getPM)

public:
  explicit TAlgorithm(QObject *parent = 0);
  ~TAlgorithm();

  // �������� ������� ������.
  // ��� ���������� ������� ������ IObject � ������ name
  //  � ��������� ������ obj � ������ � ������ name.
  // ���� name �� ������, �� � �������� ����� ����� QObject::objectName().
  // ������ false, ���� ��� �� ������ ��� ����������, ����� true.
  bool addObject(QObject *obj, const QString &name);

  ObjectManager *getOM() const;

  PluginManager *getPM() const;

  MemoryManager *getMM() const;

signals:
  void addResult(const QString &str);
  void start();

public slots:
  QObject *GetObject(const QString &name);


private:
  MemoryManager *MM_ = 0;
  ObjectManager *OM_ = 0;// �������� ��������
  PluginManager *PM_ = 0;  // �������� ��������
};

#endif // TALGORITHM_H
