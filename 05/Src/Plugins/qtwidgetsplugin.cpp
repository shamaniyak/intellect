#include "qtwidgetsplugin.h"
#include <QtWidgets>

//Q_SCRIPT_DECLARE_QMETAOBJECT(QLabel, QWidget*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(QLineEdit, QWidget*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(QPushButton, QWidget*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(QGridLayout, QWidget*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(QWidget, QWidget*)

Q_DECLARE_METATYPE(QWidget*)
Q_DECLARE_METATYPE(QMainWindow*)

QScriptValue QMainWindowToScriptValue(QScriptEngine *engine, QMainWindow* const &in)
{ return engine->newQObject(in); }

void QMainWindowFromScriptValue(const QScriptValue &object, QMainWindow* &out)
{ out = qobject_cast<QMainWindow*>(object.toQObject()); }

//  Qt::MouseButton

Q_DECLARE_METATYPE(Qt::MouseButton)

QScriptValue QtMouseButtonToScriptValue(QScriptEngine *engine, Qt::MouseButton const &in)
{ return engine->newVariant(QVariant(int(in))); }

void QtMouseButtonFromScriptValue(const QScriptValue &object, Qt::MouseButton &out)
{ out = (Qt::MouseButton)(object.toInt32()); }

//  QPointF

Q_DECLARE_METATYPE(QPointF)

QScriptValue QPointFToScriptValue(QScriptEngine *engine, QPointF const &in)
{
  QScriptValue obj = engine->newObject();
  obj.setProperty("x", in.x());
  obj.setProperty("y", in.y());
  return obj;
}

void QPointFFromScriptValue(const QScriptValue &object, QPointF &out)
{
  out.setX(object.property("x").toNumber());
  out.setY(object.property("y").toNumber());
}

// QtWidgetsPlugin

QtWidgetsPlugin::QtWidgetsPlugin(QScriptEngine *engine)
{
  if(engine){
//    QScriptValue metaObj = engine->scriptValueFromQMetaObject<QLabel>();
//    engine->globalObject().setProperty("QLabel", metaObj);

//    metaObj = engine->scriptValueFromQMetaObject<QLineEdit>();
//    engine->globalObject().setProperty("QLineEdit", metaObj);

//    metaObj = engine->scriptValueFromQMetaObject<QPushButton>();
//    engine->globalObject().setProperty("QPushButton", metaObj);

//    metaObj = engine->scriptValueFromQMetaObject<QGridLayout>();
//    engine->globalObject().setProperty("QGridLayout", metaObj);

//    metaObj = engine->scriptValueFromQMetaObject<QWidget>();
//    engine->globalObject().setProperty("QWidget", metaObj);

    qScriptRegisterMetaType(engine, QMainWindowToScriptValue, QMainWindowFromScriptValue);
    qScriptRegisterMetaType(engine, QtMouseButtonToScriptValue, QtMouseButtonFromScriptValue);
  }
}
