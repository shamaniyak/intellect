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

public:
  explicit Intellect(QObject *parent = 0);
  ~Intellect();

  IObject* obj() const;

  void setMainWindow(QMainWindow *mw);

  QString compileText(const QString &str);

  int loadPlugins();
  void addPlugin(QObject *obj);

  void event(QObject *obj, QEvent *ev);

public slots:
  void stop();

protected slots:
  void on_addObject(QObject *obj);
  void on_start();

protected:
  bool keyEvent(QObject *obj, QEvent *ev);

private:
  IObject *obj_ =0;
};

#endif // INTELLECT_H
