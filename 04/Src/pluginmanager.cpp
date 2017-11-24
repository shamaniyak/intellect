#include "pluginmanager.h"

#include <QDir>
#include <QApplication>
#include <QMessageBox>
#include <QPluginLoader>

PluginManager::PluginManager(QObject *parent) : QObject(parent)
{
  //loadPlugins();
}

PluginManager::~PluginManager()
{

}

void PluginManager::clear()
{

}

void PluginManager::loadPlugins()
{
  QDir dir(QApplication::applicationDirPath());
  if (!dir.cd("plugins")) {
    QMessageBox::critical(0, "", "plugins directory does not exist");
    return;
  }
  foreach (QString fileName, dir.entryList(QDir::Files)) {
    QPluginLoader loader(dir.absoluteFilePath(fileName));
    addPlugin(qobject_cast<QObject*>(loader.instance()));
  }
}

void PluginManager::addPlugin(QObject *pobj)
{
  if(pobj)
    pobj->setParent(this);
}

int PluginManager::count() const
{
  return(this->children().size());
}

QObject *PluginManager::get(unsigned int i) const
{
  if(i < (unsigned int)count())
    return(this->children()[i]);
  return nullptr;
}

QObject *PluginManager::getByName(const QString &name)
{
  return(this->findChild<QObject*>(name) );
}
