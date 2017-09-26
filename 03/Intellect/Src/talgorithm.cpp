#include "iobject.h"
#include "talgorithm.h"

TAlgorithm::TAlgorithm(QObject *parent) : QObject(parent),
  MM_(new MemoryManager(this)),
  OM_(new ObjectManager(this)),
  PM_(new PluginManager(this)),
  WM_(new WindowManager(this))
{
}

TAlgorithm::~TAlgorithm()
{

}

bool TAlgorithm::addObject(QObject *obj, const QString &name)
{
  QObject *newObj = GetObject(name);
  IObject *newIObj = qobject_cast<IObject*>(newObj);

  if(newIObj) {
    return( newIObj->addObject(obj, name) );
  }

  return false;
}

QObject *TAlgorithm::GetObject(const QString &name)
{
  return OM_->Get(name);
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
