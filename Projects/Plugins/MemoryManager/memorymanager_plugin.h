//#pragma once
#ifndef MEMORYMANAGER_PLUGIN_H
#define MEMORYMANAGER_PLUGIN_H

#include <QQmlExtensionPlugin>

class MemoryManagerPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri);
};

#endif//MEMORYMANAGER_PLUGIN_H
