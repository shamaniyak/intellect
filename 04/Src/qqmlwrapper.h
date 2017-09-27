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

  virtual bool execute() override;

  bool addObject(QObject *_o, const QString &_name=QString()) override;

private:
  QQmlEngine m_qml;
};

#endif // QQMLWRAPPER_H
