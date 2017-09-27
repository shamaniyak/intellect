#ifndef WORKER_H
#define WORKER_H

#include <QObject>

#include "talgorithm.h"

class Worker : public TAlgorithm
{
  Q_OBJECT
public:
  explicit Worker(QObject *parent = 0);

signals:

public slots:

private:
  IObject *obj_ =0;
};

#endif // WORKER_H
