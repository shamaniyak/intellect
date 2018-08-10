#include "qqmlwrapper.h"

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

QQmlApplicationEngine *QmlEngineWrapper::engine;

QmlEngineWrapper::QmlEngineWrapper(QObject *parent) : QObject(parent)
{
}

QmlEngineWrapper::~QmlEngineWrapper()
{

}

QmlEngineWrapper::QmlEngineWrapper(const QmlEngineWrapper &val) : QObject(val.parent())
{
  *this = val;
}

QmlEngineWrapper &QmlEngineWrapper::operator =(const QmlEngineWrapper &)
{
  return *this;
}

bool QmlEngineWrapper::evaluate(const QString &txt, const QString &path)
{
  m_msg = "";
  //engine->loadData(text().toLocal8Bit());
  //m_qml.collectGarbage();
  createQmlEngine();
  insertObjectsInQml();

  QQmlComponent component(m_qml);
	component.setData(txt.toLocal8Bit(), QUrl(path));
  if(component.isError())
    m_msg = component.errorString();
  else {
    m_tempObject = component.create();
    QObject::connect(m_tempObject, &QObject::destroyed, &dbgObj, &DebugObject::onObjectDestroyed);
  }

  return !component.isError();
}

bool QmlEngineWrapper::addObject(QObject *_o, const QString &_name)
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

QObject *QmlEngineWrapper::getObject(const QString &name) const
{
  if(m_objects.find(name) !=m_objects.end())
    return m_objects[name];

  return nullptr;
}

void QmlEngineWrapper::init()
{
  //QmlRegisterTypes::init();

  engine = new QQmlApplicationEngine();
  //engine->loadData("import QtQml 2.0\nText { text: \"Hello world!\" }");
}

QString QmlEngineWrapper::msg() const
{
  return m_msg;
}

void QmlEngineWrapper::reset()
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

void QmlEngineWrapper::addImportPath(const QString &path)
{
  createQmlEngine();
  if(m_qml) {
    m_qml->addImportPath(path);
  }
}

void QmlEngineWrapper::load(const QString &path)
{
  createQmlEngine();
  QQmlComponent component(m_qml);
  component.loadUrl(path);
  if(component.isError())
    m_msg = component.errorString();
  else {
    m_tempObject = component.create();
    QObject::connect(m_tempObject, &QObject::destroyed, &dbgObj, &DebugObject::onObjectDestroyed);
  }
}

void QmlEngineWrapper::insertObjectsInQml()
{
  auto it = m_objects.begin();
  while(it != m_objects.end()) {
    m_qml->rootContext()->setContextProperty(it.key(), it.value());

    ++it;
  }
}

void QmlEngineWrapper::createQmlEngine()
{
  if(!m_qml)
    m_qml = new QQmlEngine();
}
