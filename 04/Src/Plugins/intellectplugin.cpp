#include "intellectplugin.h"
#include "Src/iobject.h"
#include "Src/objectmanager.h"
#include "Src/windowmanager.h"

#include <QtScript>
#include <QJSEngine>
//

QScriptValue IObjectToScriptValue(QScriptEngine *engine, IObject* const &in)
{ return engine->newQObject(in); }

void IObjectFromScriptValue(const QScriptValue &object, IObject* &out)
{ out = qobject_cast<IObject*>(object.toQObject()); }

//
Q_DECLARE_METATYPE(ObjectManager*)

QScriptValue ObjectManagerToScriptValue(QScriptEngine *engine, ObjectManager* const &in)
{ return engine->newQObject(in); }

void ObjectManagerFromScriptValue(const QScriptValue &object, ObjectManager* &out)
{ out = qobject_cast<ObjectManager*>(object.toQObject()); }

//
Q_DECLARE_METATYPE(WindowManager*)

QScriptValue WindowManagerToScriptValue(QScriptEngine *engine, WindowManager* const &in)
{ return engine->newQObject(in); }

void WindowManagerFromScriptValue(const QScriptValue &object, WindowManager* &out)
{ out = qobject_cast<WindowManager*>(object.toQObject()); }



IntellectPlugin::IntellectPlugin(QScriptEngine *engine)
{
  if(engine){
    qScriptRegisterMetaType(engine, IObjectToScriptValue, IObjectFromScriptValue);
    qScriptRegisterMetaType(engine, ObjectManagerToScriptValue, ObjectManagerFromScriptValue);
    qScriptRegisterMetaType(engine, WindowManagerToScriptValue, WindowManagerFromScriptValue);
  }
}

QJSIntellectPlugin::QJSIntellectPlugin(QJSEngine *engine)
{
  if(engine)
  {

  }
}
