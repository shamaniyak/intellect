#include "pluginmanager.h"

#include <QtDebug>
#include <QDir>
#include <QApplication>
#include <QPluginLoader>

PluginManager::PluginManager(QObject *parent) : QObject(parent)
{
  //loadPlugins();
}

PluginManager::~PluginManager()
{
  clear();
}

void PluginManager::clear()
{
  for(auto &&obj: children())
  {
    auto p = qobject_cast<QPluginLoader*>(obj);
    if(p)
      p->unload();
  }
}

void PluginManager::loadPlugins()
{
  QDir dir(QApplication::applicationDirPath());
  if (!dir.cd("plugins")) {
    qDebug() << "PluginManager: plugins directory does not exist";
    return;
  }
  for (QString fileName: dir.entryList(QDir::Files)) {
    // Уберем расширение
    auto pos = fileName.lastIndexOf('.');
    if(pos > -1)
      fileName = fileName.left(pos);
    loadPlugin(dir, fileName);
  }
}

int PluginManager::count() const
{
  return(this->children().size());
}

QObject *PluginManager::get(unsigned int i) const
{
  if(i < (unsigned int)count())
  {
    auto pl = qobject_cast<QPluginLoader*>(this->children()[i]);
    if(pl)
      return(pl->instance());
  }
  return nullptr;
}

QObject *PluginManager::getByName(const QString &name)
{
  auto pl = this->findChild<QPluginLoader*>(name);
  return( pl ? pl->instance() : nullptr );
}

QObject *PluginManager::loadPlugin(const QString &name)
{
  auto pobj = getByName(name);
  if(pobj)
    return pobj;

  QDir dir(QApplication::applicationDirPath());
  if (!dir.cd("plugins")) {
    qDebug() << "PluginManager: plugins directory does not exist";
    return nullptr;
  }

  return loadPlugin(dir, name);
}

bool PluginManager::unloadPlugin(const QString &name)
{
  auto pobj = this->findChild<QPluginLoader*>(name);
  if(pobj) {
    if(pobj->unload()) {
      delete pobj;
      return true;
    }
  }
  return false;
}

void PluginManager::addPlugin(QObject *pobj)
{
  pobj = qobject_cast<QObject*>(pobj);
  if(pobj)
    pobj->setParent(this);
}

QObject *PluginManager::loadPlugin(const QDir &dir, const QString &name)
{
  QPluginLoader *loader = new QPluginLoader(dir.absoluteFilePath(name));
  auto pobj = loader->instance();
  if(pobj)
  {
    loader->setObjectName(name);
    loader->setParent(this);
  }
  return pobj;
}
