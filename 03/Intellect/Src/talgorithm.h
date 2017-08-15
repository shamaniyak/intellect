#ifndef TALGORITHM_H
#define TALGORITHM_H

#include <QObject>
#include "objectmanager.h"
#include "pluginmanager.h"

// Реализация алгоритма
class TAlgorithm : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QObject* OM READ OM)
  Q_PROPERTY(QObject* PM READ PM)

public:
  explicit TAlgorithm(QObject *parent = 0);
  ~TAlgorithm();

  // Добавить внешний объект.
  // При добавлении создает объект IObject с именем name
  //  и добавляет объект obj в скрипт с именем name.
  // Если name не задано, то в качестве имени будет QObject::objectName().
  // Вернет false, если имя не задано или существует, иначе true.
  bool addObject(QObject *obj, const QString &name);

  ObjectManager *OM() const;

  PluginManager *PM() const;

signals:
  void addResult(const QString &str);
  void start();

public slots:
  QObject *GetObject(const QString &name);


private:
  ObjectManager *OM_ = 0;// Мэнеджер объектов
  PluginManager *PM_;  // Мэнеджер плагинов
};

#endif // TALGORITHM_H
