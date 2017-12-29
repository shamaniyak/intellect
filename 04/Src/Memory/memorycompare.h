#ifndef MEMORYCOMPARE_H
#define MEMORYCOMPARE_H

#include <QObject>

class MemoryCompare : public QObject
{
  Q_OBJECT
public:
  explicit MemoryCompare(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MEMORYCOMPARE_H