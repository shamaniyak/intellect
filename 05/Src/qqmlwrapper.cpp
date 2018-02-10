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

static DebugObject dbgObj;

QQmlApplicationEngine *QQmlEngineWrapper::engine;

QQmlEngineWrapper::QQmlEngineWrapper(QObject *parent) : QObject(parent)
{
  //QmlMemoryPlugin *plugin = new QmlMemoryPlugin();
  //QJSEngineMemoryPlugin p1(m_qml);
}

QQmlEngineWrapper::~QQmlEngineWrapper()
{

}

QQmlEngineWrapper::QQmlEngineWrapper(const QQmlEngineWrapper &val) : QObject(val.parent())
{
  *this = val;
}

QQmlEngineWrapper &QQmlEngineWrapper::operator =(const QQmlEngineWrapper &)
{
  return *this;
}

bool QQmlEngineWrapper::evaluate(const QString &txt)
{
  m_msg = "";
  //engine->loadData(text().toLocal8Bit());
  //m_qml.collectGarbage();
  if(!m_qml)
    m_qml = new QQmlEngine();
  insertObjectsInQml();

  QQmlComponent component(m_qml);
  component.setData(txt.toLocal8Bit(), QUrl());
  if(component.isError())
    m_msg = component.errorString();
  else {
    m_tempObject = component.create();
    QObject::connect(m_tempObject, &QObject::destroyed, &dbgObj, &DebugObject::onObjectDestroyed);
  }

  return !component.isError();
}

bool QQmlEngineWrapper::addObject(QObject *_o, const QString &_name)
{
//  if(!m_qml)
//    return false;
  QString nm = _name;
  if(nm.isEmpty())
    nm = _o->objectName();
  if(nm.isEmpty())
    return false;

  auto o = getObject(nm);
  if(o)
    return false;

  m_objects.insert(nm, _o);

//  m_qml->rootContext()->setContextProperty(_name, _o);
  return true;
}

QObject *QQmlEngineWrapper::getObject(const QString &name) const
{
  if(m_objects.find(name) !=m_objects.end())
    return m_objects[name];

  return nullptr;
}

void QQmlEngineWrapper::init()
{
  QmlRegisterTypes::init();

  engine = new QQmlApplicationEngine();
  //engine->loadData("import QtQml 2.0\nText { text: \"Hello world!\" }");
}

QString QQmlEngineWrapper::msg() const
{
  return m_msg;
}

void QQmlEngineWrapper::reset()
{
  if(m_tempObject)
    m_tempObject->deleteLater();
  if(m_qml)
  {
    delete m_qml;
    m_qml = nullptr;
  }
  //m_qml.clearComponentCache();
  //m_qml.collectGarbage();
}

void QQmlEngineWrapper::insertObjectsInQml()
{
  auto it = m_objects.begin();
  while(it != m_objects.end()) {
    m_qml->rootContext()->setContextProperty(it.key(), it.value());

    ++it;
  }
}
