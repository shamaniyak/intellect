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
  //qmlRegisterType<MemoryWrapper>("Intellect.MemoryWrapper", 1, 0, "Memory");
  //qRegisterMetaType<MEWrapper>("MEWrapper");
  //qmlRegisterType<MEWrapper>();
  //qmlRegisterUncreatableType<MEWrapper>("Intellect.MemoryWrapper", 1, 0, "MEWrapper", "MEWrapper: can't create.");
  //qmlRegisterType<QMemoryModel>("Intellect.MemoryModel", 1, 0, "MemoryModel");
	//qmlRegisterType<MemoryTreeView>("Intellect.MemoryTreeView", 1, 0, "MemoryTreeView");
}

void QmlRegisterTypes::initJSEngine(QJSEngine *jse)
{
	auto jsv = jse->newQMetaObject<ChangeEvent>();
	jse->globalObject().setProperty("ChangeEvent", jsv);
}
