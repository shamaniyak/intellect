#ifndef MODELMEMORY_H
#define MODELMEMORY_H

#include <QObject>

class ModelMemory : public QObject
{
  Q_OBJECT
public:
  explicit ModelMemory(QObject *parent = 0);

signals:

public slots:
};

#endif // MODELMEMORY_H