#ifndef INTELLECT_H
#define INTELLECT_H

#include <memory>
#include "iobject.h"
#include "talgorithm.h"

class WindowManager;
class QMainWindow;

class Intellect : public TAlgorithm
{
  Q_OBJECT
  Q_PROPERTY(QObject* parent READ parent)
  Q_PROPERTY(IObject* obj READ obj)
  Q_PROPERTY(QObject* mainWindow READ getMainWindowQObject)

public:
  explicit Intellect(QObject *parent = 0);
  ~Intellect();

  IObject* obj() const;

  QMainWindow *getMainWindow() const;
  void setMainWindow(QMainWindow *mw);

  QString compileText(const QString &str);

  int loadPlugins();

  void event(QObject *obj, QEvent *ev);

public slots:
  void stop();

protected slots:
  void on_addObject(QObject *obj);
  void on_start();

protected:
  bool keyEvent(QObject *obj, QEvent *ev);

  QObject *getMainWindowQObject() const;

private:
  IObject *obj_ = 0;
  QMainWindow *mainWindow_ = 0;
};

#endif // INTELLECT_H
