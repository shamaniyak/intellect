#ifndef MEMORYEDITOR_H
#define MEMORYEDITOR_H

#include <QObject>

class MemoryEditor : public QObject
{
  Q_OBJECT
public:
  explicit MemoryEditor(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MEMORYEDITOR_H