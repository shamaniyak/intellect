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
  }
}
