#include "qmlregistertypes.h"

#include "Src/Memory/memorywrapper.h"
#include "Src/Memory/qmemorymodel.h"
#include "Src/GUI/MemoryView/memorytreeview.h"

#include <QQmlEngine>

QmlRegisterTypes::QmlRegisterTypes(QObject *parent) : QObject(parent)
{

}

void QmlRegisterTypes::init()
{
  qmlRegisterType<MemoryWrapper>("Intellect.MemoryWrapper", 1, 0, "Memory");
  qmlRegisterType<MEWrapper1>();
  qmlRegisterType<QMemoryModel>("Intellect.MemoryModel", 1, 0, "MemoryModel");
  qmlRegisterType<MemoryTreeView>("Intellect.MemoryTreeView", 1, 0, "MemoryTreeView");
}
