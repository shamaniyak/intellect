#include "memorymanager_plugin.h"
#include "memorymanager.h"
#include "Memory/memorywrapper.h"
#include "Memory/qmemorymodel.h"
#include "Memory/memoryeditor.h"

#include <qqml.h>

void MemoryManagerPlugin::registerTypes(const char *uri)
{
    // @uri memorymanagerplugins
    qmlRegisterType<MemoryManager>(uri, 1, 0, "MemoryManager");
    qmlRegisterType<QMemoryModel>(uri, 1, 0, "MemoryModel");

    qRegisterMetaType<MEWrapper>("MEWrapper");
    qmlRegisterUncreatableType<MEWrapper>(uri, 1, 0, "MEWrapper", "Can't create MEWrapper.");

    qmlRegisterType<MemoryEditor>(uri, 1, 0, "MemoryEditor");
}

