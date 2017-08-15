#ifndef TALGORITHM_H
#define TALGORITHM_H

#include <QObject>
#include "objectmanager.h"
#include "pluginmanager.h"

// ���������� ���������
class TAlgorithm : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QObject* OM READ OM)
  Q_PROPERTY(QObject* PM READ PM)

public:
  explicit TAlgorithm(QObject *parent = 0);
  ~TAlgorithm();

  // �������� ������� ������.
  // ��� ���������� ������� ������ IObject � ������ name
  //  � ��������� ������ obj � ������ � ������ name.
  // ���� name �� ������, �� � �������� ����� ����� QObject::objectName().
  // ������ false, ���� ��� �� ������ ��� ����������, ����� true.
  bool addObject(QObject *obj, const QString &name);

  ObjectManager *OM() const;

  PluginManager *PM() const;

signals:
  void addResult(const QString &str);
  void start();

public slots:
  QObject *GetObject(const QString &name);


private:
  ObjectManager *OM_ = 0;// �������� ��������
  PluginManager *PM_;  // �������� ��������
};

#endif // TALGORITHM_H
