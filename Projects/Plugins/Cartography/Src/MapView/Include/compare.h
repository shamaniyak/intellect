#ifndef COMPARE_H
#define COMPARE_H

#include "math.h"
//����������� ������ ���� ������������� ��������� ������� �����������. ��� �����������
//������� � ��������� ��������� � ����-���� �� �������������.
const double VERY_SMALL_VALUE = 0.000001;

//������� ���������� true ���� a ������ b (� ��������� eps), � ��������� ������ ������������ false
inline bool compareDoublesGreaterThan(double a, double b, const double eps)
{
    if ( (a-b) > eps )
        return true;
    return false;
}

//������� ���������� true ���� a ������ b (� ��������� eps), � ��������� ������ ������������ false
inline bool compareDoublesLessThan(double a, double b, const double eps)
{
    if ( (b-a) > eps )
        return true;
    return false;
}

//������� ���������� true ���� a ���������� �� b �� ����� ��� �� eps
inline bool compareDoublesEqual(double a, double b, const double eps)
{
    if ( fabs(a-b) < eps )
        return true;
    return false;
}

//������� ���������� true ���� a ������ b (� ��������� eps) ��� ���������� �� ���� �� ����� ��� �� eps
inline bool compareDoublesGreaterThanOrEqual(double a, double b, const double eps)
{
    if ( (a-b) > eps || fabs(a-b) < eps)
        return true;
    return false;
}

//������� ���������� true ���� a ������ b (� ��������� eps) ��� ���������� �� ���� �� ����� ��� �� eps
inline bool compareDoublesLessThanOrEqual(double a, double b, const double eps)
{
    if ( (b-a) > eps || fabs(a-b) < eps)
        return true;
    return false;
}

#endif
