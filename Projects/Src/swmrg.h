#ifndef CSWMRG_H
#define CSWMRG_H

#include <QMutex>
#include <QSemaphore>

class CSWMRG
{ 
public:
  CSWMRG(); // конструктор
  ~CSWMRG(); // деструктор
  void WaitToRead(); // предоставляет доступ к разделяемому ресурсу для чтения
  void WaitToWrite(); // предоставляет монопольный доступ к разделяемому ресурсу для записи
  void Done(); // вызывается по окончании работы с ресурсом

private:
  QMutex m_cs; // обеспечивает монопольный доступ к другим элементам
  QSemaphore m_hsemReaders; // "читатели" ждут на этом семафоре, если ресурс занят "писателем"
  QSemaphore m_hsemWriters; // "писатели" ждут на этом семафоре, если ресурс занят "читателем"
  int m_nWaitingReaders = 0; // число ждущих "читателей"
  int m_nWaitingWriters = 0; // число ждущих "писателей"
  // текущее число потоков, работающих с ресурсом
  // (0 — таких потоков нет, >0 — число "читателей",
  // -1 - один "писатель")
  int m_nActive = 0;

};

#endif // CSWMRG_H
