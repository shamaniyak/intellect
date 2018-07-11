#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <QQuickItem>

class MemoryManager : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(MemoryManager)

public:
    MemoryManager(QQuickItem *parent = nullptr);
    ~MemoryManager();
};

#endif // MEMORYMANAGER_H
