#include "iobject.h"
#include "talgorithm.h"

TAlgorithm::TAlgorithm(QObject *parent) : QObject(parent),
  MM_(new MemoryManager(this)),
  OM_(new ObjectManager(this)),
  PM_(new PluginManager(this)),
  WM_(new WindowManager(this)),
  qml_(new QmlManager(this))
{
}

TAlgorithm::~TAlgorithm()
{

}

bool TAlgorithm::addObject(QObject *obj, const QString &name)
{
  IObject *iobj = getObject(name);

  if(iobj) {
    return( iobj->addObject(obj, name) );
  }

  return false;
}

IObject *TAlgorithm::getObject(const QString &name)
{
  return qobject_cast<IObject*>(OM_->Get(name));
}

QmlManager *TAlgorithm::getQml() const
{
    return qml_;
}

MemoryManager *TAlgorithm::getMM() const
{
    return MM_;
}

WindowManager *TAlgorithm::getWM() const
{
  return WM_;
}

PluginManager *TAlgorithm::getPM() const
{
  return PM_;
}

ObjectManager *TAlgorithm::getOM() const
{
    return OM_;
}
