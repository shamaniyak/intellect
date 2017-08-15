#include "pluginmanager.h"

#include <QDir>
#include <QApplication>
#include <QMessageBox>
#include <QPluginLoader>

PluginManager::PluginManager(QObject *parent) : QObject(parent)
{
  loadPlugins();
}

PluginManager::~PluginManager()
{
  clear();
}

void PluginManager::clear()
{
//  foreach (auto x, this->plugins_) {
//    delete x;
//  }
//  this->plugins_.clear();
}

void PluginManager::loadPlugins()
{
  QDir dir(QApplication::applicationDirPath());
  if (!dir.cd("plugins")) {
    QMessageBox::critical(0, "", "plugins directory does not exist");
    return;
  }
  foreach (QString strFileName, dir.entryList(QDir::Files)) {
    QPluginLoader loader(dir.absoluteFilePath(strFileName));
    addPlugin(qobject_cast<QObject*>(loader.instance()));
  }
}

void PluginManager::addPlugin(QObject *pobj)
{
  if(pobj)
    pobj->setParent(this);
  //pobj->setParent(this);
//  if(pobj)
//    this->plugins_.push_back(pobj);
}

int PluginManager::count() const
{
  //return(plugins_.size());
  return(this->children().size());
}

QObject *PluginManager::get(unsigned int i) const
{
  if(i < (unsigned int)count())
    return(this->children()[i]);
    //return(plugins_[i]);
  return nullptr;
}

QObject *PluginManager::getByName(const QString &name)
{
  return(this->findChild<QObject*>(name) );
}
