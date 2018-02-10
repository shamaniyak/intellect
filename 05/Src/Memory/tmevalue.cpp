#include "tmevalue.h"

namespace Memory
{

TMEValue::TMEValue(const TMEValue &mev)
{
  *this = mev;
}

TMEValue::TMEValue(QVariant val):
  val_(val)
{

}

TMEValue &TMEValue::operator=(const TMEValue &mev)
{
  val_ = mev.val_;
  return *this;
}

QVariant TMEValue::val() const
{
  return val_;
}

void TMEValue::setVal(const QVariant &val)
{
  val_ = val;
}

void TMEValue::load(QDataStream &ds)
{
  //val_.load(ds);
  ds >> val_;
}

void TMEValue::save(QDataStream &ds) const
{
  //val_.save(ds);
  ds << val_;
}

}
