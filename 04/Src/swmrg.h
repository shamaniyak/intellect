#ifndef CSWMRG_H
#define CSWMRG_H

#include <QMutex>
#include <QSemaphore>

class CSWMRG
{ 
public:
  CSWMRG(); // �����������
  ~CSWMRG(); // ����������
  void WaitToRead(); // ������������� ������ � ������������ ������� ��� ������
  void WaitToWrite(); // ������������� ����������� ������ � ������������ ������� ��� ������
  void Done(); // ���������� �� ��������� ������ � ��������

private:
  QMutex m_cs; // ������������ ����������� ������ � ������ ���������
  QSemaphore m_hsemReaders; // "��������" ���� �� ���� ��������, ���� ������ ����� "���������"
  QSemaphore m_hsemWriters; // "��������" ���� �� ���� ��������, ���� ������ ����� "���������"
  int m_nWaitingReaders = 0; // ����� ������ "���������"
  int m_nWaitingWriters = 0; // ����� ������ "���������"
  // ������� ����� �������, ���������� � ��������
  // (0 � ����� ������� ���, >0 � ����� "���������",
  // -1 - ���� "��������")
  int m_nActive = 0;

};

#endif // CSWMRG_H
