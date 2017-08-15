#ifndef COMPARE_H
#define COMPARE_H

#include "math.h"
//Коэффициент введен дабы незначительно расширить границы ограничений. Для номенклатур
//близких к граничным значениям и чуть-чуть не вписывающихся.
const double VERY_SMALL_VALUE = 0.000001;

//Функция возвращает true если a больше b (с точностью eps), в противном случае возвращается false
inline bool compareDoublesGreaterThan(double a, double b, const double eps)
{
    if ( (a-b) > eps )
        return true;
    return false;
}

//Функция возвращает true если a меньше b (с точностью eps), в противном случае возвращается false
inline bool compareDoublesLessThan(double a, double b, const double eps)
{
    if ( (b-a) > eps )
        return true;
    return false;
}

//Функция возвращает true если a отличается от b не более чем на eps
inline bool compareDoublesEqual(double a, double b, const double eps)
{
    if ( fabs(a-b) < eps )
        return true;
    return false;
}

//Функция возвращает true если a больше b (с точностью eps) или отличается от него не более чем на eps
inline bool compareDoublesGreaterThanOrEqual(double a, double b, const double eps)
{
    if ( (a-b) > eps || fabs(a-b) < eps)
        return true;
    return false;
}

//Функция возвращает true если a меньше b (с точностью eps) или отличается от него не более чем на eps
inline bool compareDoublesLessThanOrEqual(double a, double b, const double eps)
{
    if ( (b-a) > eps || fabs(a-b) < eps)
        return true;
    return false;
}

#endif
