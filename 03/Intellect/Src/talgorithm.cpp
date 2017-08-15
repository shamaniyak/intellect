#include "iobject.h"
#include "talgorithm.h"

TAlgorithm::TAlgorithm(QObject *parent) : QObject(parent),
  OM_(new ObjectManager(this)),
  PM_(new PluginManager(this))
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

PluginManager *TAlgorithm::PM() const
{
    return PM_;
}

ObjectManager *TAlgorithm::OM() const
{
    return OM_;
}
