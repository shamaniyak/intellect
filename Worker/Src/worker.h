#ifndef WORKER_H
#define WORKER_H

#include <memory>
#include <Src/iobject.h>
#include <Src/talgorithm.h>

class WindowManager;
class QMainWindow;

class Worker : public TAlgorithm
{
  Q_OBJECT
  Q_PROPERTY(QObject* parent READ parent)
  Q_PROPERTY(IObject* obj READ obj)
  Q_PROPERTY(QObject* mainWindow READ getMainWindowQObject)

public:
  explicit Worker(QObject *parent = 0);
  ~Worker();

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

#endif // WORKER_H
