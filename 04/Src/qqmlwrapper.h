#ifndef QQMLWRAPPER_H
#define QQMLWRAPPER_H

#include "scriptwrapper.h"
#include <QObject>
#include <QtQml>

//
class QQmlEngineWrapper : public ScriptWrapper
{
public:
  explicit QQmlEngineWrapper(QQmlEngineWrapper *parent=nullptr);
  ~QQmlEngineWrapper();

  QQmlEngineWrapper(const QQmlEngineWrapper &val);
  QQmlEngineWrapper & operator =(const QQmlEngineWrapper &);

  // ScriptWrapper interface
public:
  bool evaluate(const QString &txt) override;

  bool addObject(QObject *_o, const QString &_name=QString()) override;

  static void init();

private:
  QQmlEngine m_qml;
  QObject *m_tempObject = nullptr;

  static QQmlApplicationEngine *engine;

};

#endif // QQMLWRAPPER_H
