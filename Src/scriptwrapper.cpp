#include "scriptwrapper.h"
#include <QMainWindow>
#include <Src/Plugins/qmlregistertypes.h>

ScriptWrapper::ScriptWrapper(QObject *parent) : QObject(parent)
{

}

ScriptWrapper::~ScriptWrapper()
{
}

ScriptWrapper::ScriptWrapper(const ScriptWrapper &val) :
  ScriptWrapper()
{
  *this = val;
}

ScriptWrapper &ScriptWrapper::operator =(const ScriptWrapper &)
{
  //todo: ScriptWrapper &ScriptWrapper::operator =(const ScriptWrapper &val)
  return *this;
}

void ScriptWrapper::clear()
{
}

void ScriptWrapper::initPlugins()
{
}

void ScriptWrapper::abort()
{
}

void ScriptWrapper::insertObjectsInScript()
{
}

bool ScriptWrapper::evaluate(const QString &txt)
{
    return false;
}

QString ScriptWrapper::msg() const
{
  return m_msg;
}

bool ScriptWrapper::addObject(QObject *_o, const QString &_name)
{
  QString nm = _name;
  if(nm.isEmpty())
    nm = _o->objectName();
  if(nm.isEmpty())
    return false;

  auto o = getObject(nm);
  if(o)
    return false;

  m_objects.insert(nm, _o);

  return true;
}

QObject *ScriptWrapper::getObject(const QString &name) const
{
  if(m_objects.find(name) !=m_objects.end())
    return m_objects[name];

  return nullptr;
}

void ScriptWrapper::add_objects(const ScriptWrapper *parent)
{
  if(parent!=nullptr) {
    auto it = parent->m_objects.begin();
    while(it != parent->m_objects.end()) {
      addObject(it.value(), it.key());
      ++it;
    }
  }
  else{

  }
}

void ScriptWrapper::setMsg(const QString &msg)
{
  m_msg = msg;
}

JSEngineWrapper::JSEngineWrapper(ScriptWrapper *parent) : ScriptWrapper(parent)
{
  initPlugins();
}

JSEngineWrapper::~JSEngineWrapper()
{

}

JSEngineWrapper::JSEngineWrapper(const JSEngineWrapper &val) : ScriptWrapper(val)
{
  *this = val;
}

JSEngineWrapper &JSEngineWrapper::operator =(const JSEngineWrapper &)
{
  return *this;
}

bool JSEngineWrapper::evaluate(const QString &txt)
{
  m_jsengine.collectGarbage();

	QJSValue result = m_jsengine.evaluate(txt, objectName());
  QString msg;
  if(result.isError()) {
    msg = "Line " + result.property("lineNumber").toString();
    msg = msg + ": " + result.toString();
  }
  else
    msg = result.toString();

  setMsg(msg);

  return !result.isError();
}

bool JSEngineWrapper::addObject(QObject *_o, const QString &_name)
{
  bool result = false;
  QString nm = _name;
  if(nm.isEmpty())
    nm = _o->objectName();
  if(nm.isEmpty())
    return result;

  result = ScriptWrapper::addObject(_o, nm);
  if(result)
  {
    QJSValue scriptObj = m_jsengine.newQObject(_o);
    m_jsengine.globalObject().setProperty(nm, scriptObj);
  }

  return result;
}

void JSEngineWrapper::deleteJSengine()
{
  m_jsengine.collectGarbage();
}

void JSEngineWrapper::initPlugins()
{
	QmlRegisterTypes::initJSEngine(&m_jsengine);
}

void JSEngineWrapper::insertObjectsInScript()
{
  auto it = m_objects.begin();
  while(it != m_objects.end()) {
    QJSValue scriptObj = m_jsengine.newQObject(it.value());
    m_jsengine.globalObject().setProperty(it.key(), scriptObj);

    ++it;
  }
}

