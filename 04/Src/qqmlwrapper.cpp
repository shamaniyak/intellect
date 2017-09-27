#include "qqmlwrapper.h"
#include "Plugins/memoryplugin.h"

#include <QQuickItem>

QQmlEngineWrapper::QQmlEngineWrapper(QQmlEngineWrapper *parent) : ScriptWrapper(parent)
{
  //QmlMemoryPlugin *plugin = new QmlMemoryPlugin();
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
  QQmlComponent *component = new QQmlComponent(&m_qml, QUrl(text()));

  if(component->isError())
    setMsg(component->errorString());
  return true;
}

bool QQmlEngineWrapper::addObject(QObject *_o, const QString &_name)
{
  return ScriptWrapper::addObject(_o, _name);
}
