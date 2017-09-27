#include "testsppzcartography.h"

#include <Interfaces/isppzcartographyplugin.h>
#include <Interfaces/imap.h>

#include <QtDebug>
#include <QPluginLoader>

void TestSppzCartography::test1()
{
  QPluginLoader loader("sppzcartography");
  QObject *obj = loader.instance();
  if(obj)
  {
    ISppzCartographyPlugin *pl = dynamic_cast<ISppzCartographyPlugin*>(obj);
    qDebug() << pl->getVersion();
  }
}

TestSppzCartography::TestSppzCartography()
{
  //test1();

  QLibrary lib("sppzcartography");
  if(lib.load())
  {
    qDebug() << "sppzcartography successfully loaded";
    IMap *map = nullptr;
    typedef IMap* (*FgetMap)();
    FgetMap getMap = (FgetMap)lib.resolve("getMap");
    if(getMap)
      map = getMap();
  }
  else
    qDebug() << lib.errorString();
}
