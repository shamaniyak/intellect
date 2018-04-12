#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <QObject>
#include <QMap>

class IObject;

class ObjectManager : public QObject
{
  Q_OBJECT
public:
  explicit ObjectManager(QObject *parent = 0);

  void abort();

signals:
  void signalAddObject(QObject *obj);

public slots:
  QObject *Add(const QString &name);
  QObject *Get(const QString &name);
  bool Del(const QString &name);

protected:
  void CreateObject(const QString &name);

private:
  typedef QMap<QString, IObject*> t_objects;
  t_objects objects_;
};

#endif // OBJECTMANAGER_H
