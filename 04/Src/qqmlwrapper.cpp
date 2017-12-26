#include "qqmlwrapper.h"
#include "Plugins/memoryplugin.h"

#include <QQuickItem>

QQmlApplicationEngine *QQmlEngineWrapper::engine;

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
  QString msg;
  engine->loadData(text().toLocal8Bit());
  //QQmlComponent component(engine);
  //component.setData(text().toLocal8Bit(), QUrl());
  //if(component.isError())
  //  msg = component.errorString();
  //auto result = m_qml.evaluate(text());
//  if(result.isError()) {
//    msg = "Line " + QString().setNum(result.property("lineNumber").toInt());
//    msg = msg + ": " + result.toString();
//  }
//  else
//    msg = result.toString();

  setMsg(msg);

  return true;
  //return !component.isError();
  //return !result.isError();
}

bool QQmlEngineWrapper::addObject(QObject *_o, const QString &_name)
{
  return ScriptWrapper::addObject(_o, _name);
}

void QQmlEngineWrapper::init()
{
  engine = new QQmlApplicationEngine();
  //engine->loadData("import QtQml 2.0\nText { text: \"Hello world!\" }");
}
