#ifndef CARTOGRAPHYMAP_H
#define CARTOGRAPHYMAP_H

#include <QObject>

class IMap;

class CartographyMap : public QObject
{
  Q_OBJECT
public:
  explicit CartographyMap(QObject *parent = 0);

  IMap *map() const;
  void setMap(IMap *map);

signals:
  changed();

public slots:
  void setFileMap();

protected:
  void init();

private:
  IMap *map_ = nullptr;
};

#endif // CARTOGRAPHYMAP_H
