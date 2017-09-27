#ifndef TALGORITHM_H
#define TALGORITHM_H

#include <QObject>
//#include "tmemory.h"
#include "memorymanager.h"
#include "objectmanager.h"
#include "pluginmanager.h"
#include "windowmanager.h"

// Реализация алгоритма
class TAlgorithm : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QObject* MM READ getMM)
  Q_PROPERTY(QObject* OM READ getOM)
  Q_PROPERTY(QObject* PM READ getPM)
  Q_PROPERTY(WindowManager* WM READ getWM)

public:
  explicit TAlgorithm(QObject *parent = 0);
  ~TAlgorithm();

  // Добавить внешний объект.
  // При добавлении создает объект IObject с именем name
  //  и добавляет объект obj в скрипт с именем name.
  // Если name не задано, то в качестве имени будет QObject::objectName().
  // Вернет false, если имя не задано или существует, иначе true.
  bool addObject(QObject *obj, const QString &name);

  ObjectManager *getOM() const;

  PluginManager *getPM() const;

  MemoryManager *getMM() const;

  WindowManager* getWM() const;

  IObject *getObject(const QString &name);

signals:
  void addResult(const QString &str);
  void start();

public slots:

private:
  MemoryManager *MM_ = 0;
  ObjectManager *OM_ = 0;// Мэнеджер объектов
  PluginManager *PM_ = 0;  // Мэнеджер плагинов
  WindowManager* WM_ = 0;
};

#endif // TALGORITHM_H
