#include "qqmlwrapper.h"
#include "Plugins/memoryplugin.h"

#include <QQuickItem>

QQmlEngineWrapper::QQmlEngineWrapper(QQmlEngineWrapper *parent) : ScriptWrapper(parent)
{
  //QmlMemoryPlugin *plugin = new QmlMemoryPlugin();
  QJSEngineMemoryPlugin p1(m_qml);
}

QQmlEngineWrapper::~QQmlEngineWrapper()
{

}

QQmlEngineWrapper::QQmlEngineWrapper(const QQmlEngineWrapper &val) : ScriptWrapper(val)
{
  *this = val;
}

QQmlEngineWrapper &QQmlEngineWrapper::operator =(const QQmlEngineWrapper &)
{
  return *this;
}

bool QQmlEngineWrapper::execute()
{
  QQmlComponent component(&m_qml);
  component.setData(text().toLocal8Bit(), QUrl());
  //auto result = m_qml.evaluate(text());
  QString msg;
  if(component.isError())
    msg = component.errorString();
//  if(result.isError()) {
//    msg = "Line " + QString().setNum(result.property("lineNumber").toInt());
//    msg = msg + ": " + result.toString();
//  }
//  else
//    msg = result.toString();

  setMsg(msg);

  return !component.isError();
}

bool QQmlEngineWrapper::addObject(QObject *_o, const QString &_name)
{
  return ScriptWrapper::addObject(_o, _name);
}
