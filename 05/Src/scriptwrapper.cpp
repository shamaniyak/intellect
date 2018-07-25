#include "scriptwrapper.h"
#include "Plugins/qtwidgetsplugin.h"
#include <QScriptEngineDebugger>
#include <QMainWindow>

#include "Plugins/memoryplugin.h"
#include "Plugins/intellectplugin.h"

ScriptWrapper::ScriptWrapper(QObject *parent) : QObject(parent)
{

}

ScriptWrapper::~ScriptWrapper()
{
  //deleteScript();
  //delete m_debugger;
}

ScriptWrapper::ScriptWrapper(const ScriptWrapper &val)
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
  //deleteScript();
}

void ScriptWrapper::initPlugins()
{
  //QtWidgetsPlugin plug(script());
//  MemoryPlugin plug1(m_script);
  //IntellectPlugin ip(m_script);
}

QScriptEngine *ScriptWrapper::script()
{
    return nullptr;
}

void ScriptWrapper::abort()
{
//  if(m_script)
//    m_script->abortEvaluation();
}

void ScriptWrapper::deleteScript()
{
//  m_debugger->detach();
//  if(m_script)
//  {
//    m_script->abortEvaluation();
//    delete m_script;
//    m_script = nullptr;
//  }
}

void ScriptWrapper::insertObjectsInScript()
{
//  auto it = m_objects.begin();
//  while(it != m_objects.end()) {
//    //ScriptWrapper::addObject(it.value(), it.key());
//    QScriptValue scriptObj = m_script->newQObject(it.value());
//    m_script->globalObject().setProperty(it.key(), scriptObj);

//    ++it;
//  }
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
    //m_script.setGlobalObject(parent->m_script.globalObject());
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
  //if(m_debug)
    //return ScriptWrapper::evaluate(txt);

  m_jsengine.collectGarbage();

  QJSValue result = m_jsengine.evaluate(txt);
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
    //QJSEngineMemoryPlugin p1(m_jsengine);
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

