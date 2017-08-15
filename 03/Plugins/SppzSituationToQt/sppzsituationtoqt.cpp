#include "sppzsituationtoqt.h"

#include <isituationintf.h>

#include <QLibrary>

//  PropertiesWrapper

PropertiesWrapper::PropertiesWrapper(QObject *parent) : QObject(parent)
{

}

IFigureProperties *PropertiesWrapper::props() const
{
  return props_;
}

void PropertiesWrapper::setProps(IFigureProperties *props)
{
  props_ = props;
}

int PropertiesWrapper::getCount() const
{
  if(props_)
    return(props_->count());
  return 0;
}

QString PropertiesWrapper::getName(int i) const
{
  QString name;
  if(props_)
    name = QString::fromLocal8Bit(props_->getName(i));
  return name;
}

QVariant PropertiesWrapper::getVal(int i) const
{
  QVariant val;
  if(props_)
  {
    auto v = props_->getVal(i);
    if(v) {
      val = QString::fromLocal8Bit( v->AsCString() );
//      int t = v->type();
//      switch( t )
//      {
//        case enValueType_bool:
//          val = v->AsBool();
//          break;
//        case enValueType_int:
//          val = v->AsInteger();
//          break;
//        case enValueType_double:
//          val = v->AsDouble();
//          break;
//        case enValueType_string:
//          val = QString::fromLocal8Bit( v->AsCString() );
//          break;
//        case enValueType_datetime:
//          val = QString::fromLocal8Bit( v->AsCString() );
//          break;
//        case enValueType_none:
//          val = QString::fromLocal8Bit( v->AsCString() );
//          break;
//      }
    }
  }
  return val;
}

//  ObjectWrapper

ObjectWrapper::ObjectWrapper(QObject *parent) : QObject(parent),
  props_(new PropertiesWrapper(this))
{

}

IFigureIntf *ObjectWrapper::obj() const
{
  return obj_;
}

void ObjectWrapper::setObj(IFigureIntf *obj)
{
  obj_ = obj;
}

QString ObjectWrapper::getName() const
{
  QString str;
  if(obj_)
    str = QString::fromLocal8Bit( obj_->GetNameC() );

  return str;
}

int ObjectWrapper::getClassId() const
{
  int id = -1;
  if(obj_)
    id = obj_->GetClassId();

  return id;
}

double ObjectWrapper::getPos(double &b, double &l) const
{
  if(obj_)
    return(obj_->GetPos(b, l));

  b = 0; l = 0;
  return 0;
}

double ObjectWrapper::getB() const
{
  double b, l;
  getPos(b, l);
  return b;
}

double ObjectWrapper::getL() const
{
  double b, l;
  getPos(b, l);
  return l;
}

double ObjectWrapper::getH() const
{
  double b, l, h;
  h = getPos(b, l);
  return h;
}

QObject *ObjectWrapper::getProperties()
{
  if(obj_) {
    props_->setProps(obj_->getProperties());
    return props_;
  }
  return nullptr;
}

//  SituationObjListWrapper

SituationObjListWrapper::SituationObjListWrapper(QObject *parent) : QObject(parent)
, currentObj_(new ObjectWrapper(this))
{

}

ISituationObjList *SituationObjListWrapper::list() const
{
  return list_;
}

void SituationObjListWrapper::setList(ISituationObjList *list)
{
  list_ = list;
}

int SituationObjListWrapper::getCount()
{
  if(list_)
    return( list_->getCount() );
  return 0;
}

QObject *SituationObjListWrapper::getFirst()
{
  if(list_)
    currentObj_->setObj(list_->first());

  if(currentObj_->obj())
    return currentObj_;
  return nullptr;
}

QObject *SituationObjListWrapper::getNext()
{
  if(list_)
    currentObj_->setObj(list_->next());

  if(currentObj_->obj())
    return currentObj_;
  return nullptr;
}

//  SppzSituationToQt

SppzSituationToQt::SppzSituationToQt(QObject *parent) : QObject(parent)
, objList_(new SituationObjListWrapper(this))
{
  this->setObjectName("SppzSituation");

  initLibrary();
}

SppzSituationToQt::~SppzSituationToQt()
{
  if(sit_)
    sit_->release();
  sit_ = nullptr;
}

QObject *SppzSituationToQt::getObjects()
{
  if(!sit_)
    return nullptr;

  objList_->setList( sit_->getObjects() );

  if(objList_->list())
    return objList_;
  return nullptr;
}

void SppzSituationToQt::initLibrary()
{
  if(sit_)
    return;

  QLibrary *lib = new QLibrary("d:\\Work\\sppz\\Exe\\situation", this);
  if(lib->load())
  {
    typedef ISituationIntf * (*FunCreateClassifier)();
    FunCreateClassifier fun = (FunCreateClassifier)lib->resolve("_CreateSituation");
    if(fun)
      sit_ = dynamic_cast<ISituationIntf*>( fun() );
  }

}
