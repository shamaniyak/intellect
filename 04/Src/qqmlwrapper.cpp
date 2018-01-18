#include "qqmlwrapper.h"
#include "Plugins/qmlregistertypes.h"

#include <QDebug>
#include <QQuickItem>
#include <QQuickView>

class DebugObject : public QObject
{
public:
  DebugObject(QObject * parent = Q_NULLPTR) : QObject(parent) {}

public slots:
  void onObjectDestroyed(QObject * = Q_NULLPTR)
  {
    qDebug() << "QmlObject destroyed";
  }
};

//static DebugObject dbgObj;

QQmlApplicationEngine *QQmlEngineWrapper::engine;

QQmlEngineWrapper::QQmlEngineWrapper(QQmlEngineWrapper *parent) : ScriptWrapper(parent)
{
  //QmlMemoryPlugin *plugin = new QmlMemoryPlugin();
  //QJSEngineMemoryPlugin p1(m_qml);
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

bool QQmlEngineWrapper::evaluate(const QString &txt)
{
  QString msg;
  //engine->loadData(text().toLocal8Bit());
  if(m_tempObject)
    m_tempObject->deleteLater();
  m_qml.collectGarbage();
  QQmlComponent component(&m_qml);
  component.setData(txt.toLocal8Bit(), QUrl());
  if(component.isError())
    msg = component.errorString();
  else {
    m_tempObject = component.create();
    //QObject::connect(m_tempObject, &QObject::destroyed, &dbgObj, &DebugObject::onObjectDestroyed);
  }
  //QQuickView *view = new QQuickView(&m_qml, 0);
  //view->setContent(QUrl(), &component, component.create());
  //view->show();
  //auto result = m_qml.evaluate(txt);
//  if(result.isError()) {
//    msg = "Line " + QString().setNum(result.property("lineNumber").toInt());
//    msg = msg + ": " + result.toString();
//  }
//  else
//    msg = result.toString();

  setMsg(msg);

  //return true;
  return !component.isError();
  //return !result.isError();
}

bool QQmlEngineWrapper::addObject(QObject *_o, const QString &_name)
{
  bool isAdd = ScriptWrapper::addObject(_o, _name);
  if(isAdd)
  {
    m_qml.rootContext()->setContextProperty(_name, _o);
  }
  return isAdd;
}

void QQmlEngineWrapper::init()
{
  QmlRegisterTypes::init();

  engine = new QQmlApplicationEngine();
  //engine->loadData("import QtQml 2.0\nText { text: \"Hello world!\" }");
}
