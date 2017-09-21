#ifndef CARTOGRAPHY_H
#define CARTOGRAPHY_H

#include <QObject>

class Cartography : public QObject
{
  Q_OBJECT
public:
  explicit Cartography(QObject *parent = 0);

signals:

public slots:
};

#endif // CARTOGRAPHY_H