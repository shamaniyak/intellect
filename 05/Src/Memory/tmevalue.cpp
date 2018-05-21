/****************************************************************************
**
** Copyright (C) 2015-2018 Aleksandr Abramov
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
** http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
****************************************************************************/

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
