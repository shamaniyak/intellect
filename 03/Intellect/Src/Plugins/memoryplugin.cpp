#include "memoryplugin.h"
#include "Memory/memorywrapper.h"
#include "Memory/mewrapper.h"
#include "Memory/tme.h"
#include <QtScript>
#include <QJSEngine>

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

MEWrapper* getMEWrapperFromScriptValue(const QScriptValue &object)
{
  MemoryWrapper *mem = qobject_cast<MemoryWrapper*>( object.property("mem").toQObject() );
  if(mem)
    return( mem->getById(object.property("id").toUInt32()) );
  return nullptr;
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
        me->setName(name);
    } else {
        result = me->name();
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
        me->setVal(val);
    } else {
        result = eng->newVariant(me->val());
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
    me->clear();

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
    auto me1 = me->getByI(i);
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
    auto me1 = me->add(name, checkExist);
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
    auto me1 = me->get(name);
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
    me->del(name);
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
    me->delByI(i);
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
    result = me->getIndex();
  }

  return result;
}

QScriptValue moveMe(QScriptContext *ctx, QScriptEngine *eng)
{
  QScriptValue obj = ctx->thisObject();
  checkObjectData(eng, obj);
  QScriptValue result;

  auto me = getMEWrapperFromScriptValue(obj);
  if(me && ctx->argumentCount() >0)
  {
    auto meParent = getMEWrapperFromScriptValue( ctx->argument(0).toObject() );
    int pos = ctx->argument(1).toInt32();
    me->getMem()->move(me, meParent, pos);
  }

  return result;
}

QScriptValue MEWrapperToScriptValue(QScriptEngine *engine, MEWrapper* const &in)
{
  //return engine->newQObject(in/*, QScriptEngine::ScriptOwnership*/);

  QScriptValue obj = engine->newObject();
  if(!in)
    return obj;
  if(in->deleted()) {
    engine->currentContext()->throwError("MemoryElement is deleted.");
    return obj;
  }
  //QScriptValue val = engine->newVariant(in->val());
  // поля
  obj.setProperty("name", engine->newFunction(getSetMeName),
                  QScriptValue::PropertyGetter|QScriptValue::PropertySetter);
  obj.setProperty("val", engine->newFunction(getSetMeVal),
                  QScriptValue::PropertyGetter|QScriptValue::PropertySetter);
  obj.setProperty("path", in->getPath());
  obj.setProperty("mem", engine->newQObject(in->getMem()),
                  QScriptValue::ReadOnly|QScriptValue::Undeletable);
  obj.setProperty("id", (uint)in->getMe(),
                  QScriptValue::ReadOnly|QScriptValue::Undeletable);
  //auto parent = in->parent() ? (uint)in->parent()->getMe() : 0;
  auto parent = in->parent() ? engine->toScriptValue(in->parent()) : QScriptValue();
  obj.setProperty("parent", parent,
                  QScriptValue::ReadOnly|QScriptValue::Undeletable);
  obj.setProperty("index", in->getIndex(),
                  QScriptValue::ReadOnly|QScriptValue::Undeletable);
  // функции
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
  return obj;
}

void MEWrapperFromScriptValue(const QScriptValue &object, MEWrapper* &out)
{
  out = getMEWrapperFromScriptValue(object);
}

// EMemoryChange
Q_DECLARE_METATYPE(EMemoryChange)

QScriptValue EMemoryChangeToScriptValue(QScriptEngine *engine, EMemoryChange const &in)
{ return engine->newVariant(QVariant(int(in))); }

void EMemoryChangeFromScriptValue(const QScriptValue &object, EMemoryChange &out)
{ out = (EMemoryChange)(object.toInt32()); }

MemoryPlugin::MemoryPlugin(QScriptEngine *engine)
{
  if(engine){
    qScriptRegisterMetaType(engine, MemoryWrapperToScriptValue, MemoryWrapperFromScriptValue);
    qScriptRegisterMetaType(engine, MEWrapperToScriptValue, MEWrapperFromScriptValue);
    qScriptRegisterMetaType(engine, EMemoryChangeToScriptValue, EMemoryChangeFromScriptValue);

    QScriptValue obj = engine->globalObject();

    //mcNone, mcAdd, mcAddFrom, mcDel, mcEditName, mcEditVal, mcUpdate, mcSelect, mcClear
    obj.setProperty("mcNone", mcNone, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("mcAdd", mcAdd, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("mcAddFrom", mcAddFrom, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("mcDel", mcDel, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("mcEditName", mcEditName, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("mcEditVal", mcEditVal, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("mcUpdate", mcUpdate, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("mcSelect", mcSelect, QScriptValue::ReadOnly|QScriptValue::Undeletable);
    obj.setProperty("mcClear", mcClear, QScriptValue::ReadOnly|QScriptValue::Undeletable);
  }
}


QJSValue EMemoryChangeToJSValue(QJSEngine *engine, EMemoryChange const &in)
{ return engine->toScriptValue<int>(int(in)); }

void EMemoryChangeFromJSValue(const QJSValue &object, EMemoryChange &out)
{ out = (EMemoryChange)(object.toInt()); }

QJSEngineMemoryPlugin::QJSEngineMemoryPlugin(QJSEngine &engine)
{
  //qScriptRegisterMetaType(engine, MemoryWrapperToScriptValue, MemoryWrapperFromScriptValue);
  //qScriptRegisterMetaType(engine, MEWrapperToScriptValue, MEWrapperFromScriptValue);
  //qScriptRegisterMetaType(engine, EMemoryChangeToJSValue, EMemoryChangeFromJSValue);
  //qScriptRegisterMetaType(engine, tmeToScriptValue, tmeFromScriptValue);

  QJSValue obj = engine.globalObject();

  //mcNone, mcAdd, mcAddFrom, mcDel, mcEditName, mcEditVal, mcUpdate, mcSelect, mcClear
  obj.setProperty("mcNone", mcNone);
  obj.setProperty("mcAdd", mcAdd);
  obj.setProperty("mcAddFrom", mcAddFrom);
  obj.setProperty("mcDel", mcDel);
  obj.setProperty("mcEditName", mcEditName);
  obj.setProperty("mcEditVal", mcEditVal);
  obj.setProperty("mcUpdate", mcUpdate);
  obj.setProperty("mcSelect", mcSelect);
  obj.setProperty("mcClear", mcClear);
}
