#include "memoryplugin.h"
#include "Src/Memory/memorywrapper.h"
#include "Src/Memory/mewrapper.h"
#include "Src/Memory/tme.h"
#include <QtScript>
#include <QQmlEngine>
#include <src/Memory/qmemorymodel.h>

//  MemoryWrapper

QScriptValue MemoryWrapperToScriptValue(QScriptEngine *engine, MemoryWrapper* const &in)
{ return engine->newQObject(in); }

void MemoryWrapperFromScriptValue(const QScriptValue &object, MemoryWrapper* &out)
{ out = qobject_cast<MemoryWrapper*>(object.toQObject()); }

//  MEWrapper

void checkObjectData(QScriptEngine *eng, QScriptValue &obj)
{
  QScriptValue data = obj.data();
  if (!data.isValid()) {
      data = eng->newObject();
      obj.setData(data);
  }
}

MEWrapper getMEWrapperFromScriptValue(const QScriptValue &object)
{
  MemoryWrapper *mem = qobject_cast<MemoryWrapper*>( object.property("mem").toQObject() );
  if(mem)
    return( mem->getById(object.property("id").toUInt32()) );
  return MEWrapper();
}

QScriptValue getSetMeName(QScriptContext *ctx, QScriptEngine *eng)
{
    QScriptValue obj = ctx->thisObject();
    checkObjectData(eng, obj);
    QScriptValue result;

    auto me = getMEWrapperFromScriptValue(obj);
    if(!me)
      return result;

    if (ctx->argumentCount() == 1) {
        auto name = ctx->argument(0).toString();
        me.setName(name);
    } else {
        result = me.name();
    }
    return result;
}

QScriptValue getSetMeVal(QScriptContext *ctx, QScriptEngine *eng)
{
    QScriptValue obj = ctx->thisObject();
    checkObjectData(eng, obj);
    QScriptValue result;

    auto me = getMEWrapperFromScriptValue(obj);
    if(!me)
      return result;

    if (ctx->argumentCount() == 1) {
        QVariant val = ctx->argument(0).toVariant();
        me.setVal(val);
    } else {
        result = eng->newVariant(me.val());
    }
    return result;
}

QScriptValue clearMe(QScriptContext *ctx, QScriptEngine *eng)
{
  QScriptValue obj = ctx->thisObject();
  checkObjectData(eng, obj);
  QScriptValue result;

  auto me = getMEWrapperFromScriptValue(obj);
  if(me)
    me.clear();

  return result;
}

QScriptValue getMeByI(QScriptContext *ctx, QScriptEngine *eng)
{
  QScriptValue obj = ctx->thisObject();
  checkObjectData(eng, obj);
  QScriptValue result;

  auto me = getMEWrapperFromScriptValue(obj);
  if(me && ctx->argumentCount() >0)
  {
    auto i = ctx->argument(0).toInt32();
    auto me1 = me.getByI(i);
    if(me1)
      result = eng->toScriptValue(me1);
  }

  return result;
}

QScriptValue addChildMe(QScriptContext *ctx, QScriptEngine *eng)
{
  QScriptValue obj = ctx->thisObject();
  checkObjectData(eng, obj);
  QScriptValue result;

  auto me = getMEWrapperFromScriptValue(obj);
  if(me)
  {
    auto name = ctx->argument(0).toString();
    auto checkExist = (ctx->argumentCount() >1) ? ctx->argument(1).toBool() : true;
    auto me1 = me.add(name, checkExist);
    if(me1)
      result = eng->toScriptValue(me1);
  }

  return result;
}

QScriptValue getChildMe(QScriptContext *ctx, QScriptEngine *eng)
{
  QScriptValue obj = ctx->thisObject();
  checkObjectData(eng, obj);
  QScriptValue result;

  auto me = getMEWrapperFromScriptValue(obj);
  if(me)
  {
    auto name = ctx->argument(0).toString();
    auto me1 = me.get(name);
    if(me1)
      result = eng->toScriptValue(me1);
  }

  return result;
}

QScriptValue delChildMe(QScriptContext *ctx, QScriptEngine *eng)
{
  QScriptValue obj = ctx->thisObject();
  checkObjectData(eng, obj);
  QScriptValue result;

  auto me = getMEWrapperFromScriptValue(obj);
  if(me)
  {
    auto name = ctx->argument(0).toString();
    me.del(name);
  }

  return result;
}

QScriptValue delByIndexChildMe(QScriptContext *ctx, QScriptEngine *eng)
{
  QScriptValue obj = ctx->thisObject();
  checkObjectData(eng, obj);
  QScriptValue result;

  auto me = getMEWrapperFromScriptValue(obj);
  if(me && ctx->argumentCount() >0)
  {
    auto i = ctx->argument(0).toInt32();
    me.delByI(i);
  }

  return result;
}

QScriptValue indexMe(QScriptContext *ctx, QScriptEngine *eng)
{
  QScriptValue obj = ctx->thisObject();
  checkObjectData(eng, obj);
  QScriptValue result;

  auto me = getMEWrapperFromScriptValue(obj);
  if(me)
  {
    result = me.getIndex();
  }

  return result;
}

QScriptValue moveMe(QScriptContext *ctx, QScriptEngine *eng)
{
  QScriptValue obj = ctx->thisObject();
  checkObjectData(eng, obj);
  QScriptValue result = false;

  auto me = getMEWrapperFromScriptValue(obj);
  if(me && ctx->argumentCount() >0)
  {
    auto meParent = me.parent();// getMEWrapperFromScriptValue( ctx->argument(0).toObject() );
    int pos = ctx->argument(1).toInt32();
    result = me.getMem()->move(me, meParent, pos);
  }

  return result;
}

QScriptValue getCountChildren(QScriptContext *ctx, QScriptEngine *eng)
{
  QScriptValue obj = ctx->thisObject();
  checkObjectData(eng, obj);
  QScriptValue result = 0;

  auto me = getMEWrapperFromScriptValue(obj);
  if(me)
  {
    result = me.count();
  }

  return result;
}

QScriptValue MEWrapperToScriptValue(QScriptEngine *engine, MEWrapper const &in)
{
  //return engine->newQObject(in/*, QScriptEngine::ScriptOwnership*/);

  QScriptValue obj = engine->newObject();
  if(in.isNull()) {
    engine->currentContext()->throwError("MemoryElement is deleted.");
    return obj;
  }
  //QScriptValue val = engine->newVariant(in->val());
  // ����
  obj.setProperty("name", engine->newFunction(getSetMeName),
                  QScriptValue::PropertyGetter|QScriptValue::PropertySetter);
  obj.setProperty("val", engine->newFunction(getSetMeVal),
                  QScriptValue::PropertyGetter|QScriptValue::PropertySetter);
  obj.setProperty("path", in.getPath());
  obj.setProperty("mem", engine->newQObject(in.getMem()),
                  QScriptValue::ReadOnly|QScriptValue::Undeletable);
  obj.setProperty("id", (uint)in.getMe(),
                  QScriptValue::ReadOnly|QScriptValue::Undeletable);
  //auto parent = in->parent() ? (uint)in->parent()->getMe() : 0;
  auto parent = in.parent() ? engine->toScriptValue(in.parent()) : QScriptValue();
  obj.setProperty("parent", parent,
                  QScriptValue::ReadOnly|QScriptValue::Undeletable);
  obj.setProperty("index", in.getIndex(),
                  QScriptValue::ReadOnly|QScriptValue::Undeletable);
  // �������
  obj.setProperty("clear", engine->newFunction(clearMe),
                  QScriptValue::Undeletable);
  obj.setProperty("getByI", engine->newFunction(getMeByI),
                  QScriptValue::Undeletable);
  obj.setProperty("add", engine->newFunction(addChildMe),
                  QScriptValue::Undeletable);
  obj.setProperty("get", engine->newFunction(getChildMe),
                  QScriptValue::Undeletable);
  obj.setProperty("del", engine->newFunction(delChildMe),
                  QScriptValue::Undeletable);
  obj.setProperty("delByI", engine->newFunction(delByIndexChildMe),
                  QScriptValue::Undeletable);
  obj.setProperty("move", engine->newFunction(moveMe),
                  QScriptValue::Undeletable);
  obj.setProperty("count", engine->newFunction(getCountChildren),
                  QScriptValue::Undeletable);
  return obj;
}

void MEWrapperFromScriptValue(const QScriptValue &object, MEWrapper &out)
{
  out = getMEWrapperFromScriptValue(object);
}

// EMemoryChange
Q_DECLARE_METATYPE(EMemoryChange)

QScriptValue EMemoryChangeToScriptValue(QScriptEngine *engine, EMemoryChange const &in)
{ return engine->newVariant(QVariant(int(in))); }

void EMemoryChangeFromScriptValue(const QScriptValue &object, EMemoryChange &out)
{ out = (EMemoryChange)(object.toInt32()); }

//struct ChangeEvent
//{
//  EMemoryChange type = mcNone;
//  MEWrapper *me = nullptr;
//  MEWrapper *parent = nullptr;
//  int row = 0;
//  int count = 0;
//};

QScriptValue ChangeEventToScriptValue(QScriptEngine *engine, ChangeEvent const &in)
{
  QScriptValue obj = engine->newObject();
  // ����
  QScriptValue type = EMemoryChangeToScriptValue(engine, in.type);
  QScriptValue me = MEWrapperToScriptValue(engine, in.me);
  QScriptValue parent = MEWrapperToScriptValue(engine, in.parent);
  obj.setProperty("type", type,
                  QScriptValue::ReadOnly|QScriptValue::Undeletable);
  obj.setProperty("me", me,
                  QScriptValue::ReadOnly|QScriptValue::Undeletable);
  obj.setProperty("parent", parent,
                  QScriptValue::ReadOnly|QScriptValue::Undeletable);
  obj.setProperty("row", in.row,
                  QScriptValue::ReadOnly|QScriptValue::Undeletable);
  obj.setProperty("prevName", in.prevName,
                  QScriptValue::ReadOnly|QScriptValue::Undeletable);
  obj.setProperty("prevVal", engine->newVariant(in.prevVal),
                  QScriptValue::ReadOnly|QScriptValue::Undeletable);
  obj.setProperty("path", engine->newVariant(in.path),
                  QScriptValue::ReadOnly|QScriptValue::Undeletable);
  return obj;
}

void ChangeEventFromScriptValue(const QScriptValue &object, ChangeEvent &out)
{
  out.type = EMemoryChange(object.property("type").toInt32());
  out.row = object.property("row").toInt32();
}

MemoryPlugin::MemoryPlugin(QScriptEngine *engine)
{
  if(engine){
    qScriptRegisterMetaType(engine, MemoryWrapperToScriptValue, MemoryWrapperFromScriptValue);
    qScriptRegisterMetaType(engine, MEWrapperToScriptValue, MEWrapperFromScriptValue);
    qScriptRegisterMetaType(engine, EMemoryChangeToScriptValue, EMemoryChangeFromScriptValue);
    qScriptRegisterMetaType(engine, ChangeEventToScriptValue, ChangeEventFromScriptValue);

    QScriptValue obj = engine->globalObject();

    //mcNone, mcAdd, mcAddFrom, mcDel, mcEditName, mcEditVal, mcUpdate, mcSelect, mcClear, mcMove
    obj.setProperty("mcNone", mcNone, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("mcAdd", mcAdd, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("mcAddFrom", mcAddFrom, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("mcDel", mcDel, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("mcEditName", mcEditName, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("mcEditVal", mcEditVal, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("mcUpdate", mcUpdate, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("mcSelect", mcSelect, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("mcClear", mcClear, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("mcMove", mcMove, QScriptValue::ReadOnly|QScriptValue::Undeletable);
  }
}

//  QJSEngine Plugins

// First, define the singleton type provider function (callback).
//static QJSValue example_qjsvalue_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
//{
//    Q_UNUSED(engine)

//    static int seedValue = 5;
//    QJSValue example = scriptEngine->newObject();
//    example.setProperty("someProperty", seedValue++);
//    return example;
//}

QJSEngineMemoryPlugin::QJSEngineMemoryPlugin(QJSEngine &engine)
{
  // Second, register the singleton type provider with QML by calling this function in an initialization function.
  //qmlRegisterSingletonType("Intellect.qjsvalueApi", 1, 0, "MyApi", example_qjsvalue_singletontype_provider);

  QJSValue obj = engine.globalObject();

  //mcNone, mcAdd, mcAddFrom, mcDel, mcEditName, mcEditVal, mcUpdate, mcSelect, mcClear, mcMove
  obj.setProperty("mcNone", mcNone);
  obj.setProperty("mcAdd", mcAdd);
  obj.setProperty("mcAddFrom", mcAddFrom);
  obj.setProperty("mcDel", mcDel);
  obj.setProperty("mcEditName", mcEditName);
  obj.setProperty("mcEditVal", mcEditVal);
  obj.setProperty("mcUpdate", mcUpdate);
  obj.setProperty("mcSelect", mcSelect);
  obj.setProperty("mcClear", mcClear);
  obj.setProperty("mcMove", mcMove);
}

void QJSEngineMemoryPlugin::registerTypes()
{
  //qmlRegisterType<QMemoryModel>("Intellect.MemoryWrapper", 1, 0, "Memory");
}
