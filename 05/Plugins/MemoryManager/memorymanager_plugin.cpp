#include "memorymanager_plugin.h"
#include "Memory/qmemorymodel.h"
#include "Memory/memoryeditor.h"
#include <Memory/memorycompareproxymodel.h>

#include <qqml.h>

void MemoryManagerPlugin::registerTypes(const char *uri)
{
		// @uri memorymanagerplugins
		qmlRegisterInterface<MemoryWrapper>("MemoryWrapper");
		qmlRegisterType<QMemoryModel>(uri, 1, 0, "MemoryModel");

    qRegisterMetaType<MEWrapper>("MEWrapper");
    qmlRegisterUncreatableType<MEWrapper>(uri, 1, 0, "MEWrapper", "Can't create MEWrapper.");

    qmlRegisterType<MemoryEditor>(uri, 1, 0, "MemoryEditor");
    qmlRegisterType<MemoryCompareProxyModel>(uri, 1, 0, "MemoryCompareProxyModel");
}

