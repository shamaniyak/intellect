#include "qmlmanager.h"

#include <QtQuick/QQuickView>

#include <QtQml/QQmlEngine>
#include <QtQml/qqmlcomponent.h>

QmlManager::QmlManager(QObject *parent) : QObject(parent)
{

}

QObject *QmlManager::createQmlEngine()
{
  auto qe = new QQmlEngine();
  return qe;
}

QObject *QmlManager::createQmlComponent()
{
  auto comp = new QQmlComponent();
  return comp;
}

QObject *QmlManager::createQuickView()
{
  auto qv = new QQuickView();
  return qv;
}
