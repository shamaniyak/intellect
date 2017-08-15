#include "objectmanager.h"
#include "iobject.h"

ObjectManager::ObjectManager(QObject *parent) : QObject(parent)
{

}

void ObjectManager::abort()
{
  t_objects::iterator it = objects_.begin();
  for(; it != objects_.end(); ++it)
  {
    it.value()->scr()->abort();
  }
}

void ObjectManager::CreateNewObject(const QString &name)
{
  IObject *iobj = new IObject(this);
  objects_[name] = iobj;

  iobj->setObjectName(name);
}

QObject *ObjectManager::Add(const QString &name)
{
  if(name.isEmpty())
    return nullptr;

  QString upperName = name.toUpper();

  if(!objects_.contains(upperName)) {
    CreateNewObject(upperName);
  }

  auto obj = qobject_cast<IObject*>( objects_[upperName] );
  obj->incCountLinks();

  emit signalAddObject(obj);

  return obj;
}

QObject *ObjectManager::Get(const QString &name)
{
  return Add(name);
}

bool ObjectManager::Del(const QString &name)
{
  QObject *temp = nullptr;
  QString upperName = name.toUpper();

  if(objects_.contains(upperName)) {
    temp = objects_[upperName];
    delete temp;
    objects_.remove(upperName);
  }

  return temp;
}
