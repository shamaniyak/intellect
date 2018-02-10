#ifndef TMEVALUE_H
#define TMEVALUE_H

#include <QtCore>
#include <QVariant>
#include <QDataStream>

namespace Memory
{
class TMEValue
{
public:
  TMEValue(const TMEValue &mev);
  TMEValue(QVariant val=0);

  TMEValue& operator=(const TMEValue &mev);

  QVariant val() const;
  void setVal(const QVariant &val);

  void load(QDataStream &ds);
  void save(QDataStream &ds) const;

private:
  QVariant val_;
};
}

#endif // TMEVALUE_H
