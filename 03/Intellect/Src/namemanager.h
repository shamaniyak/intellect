#ifndef NAMEMANAGER_H
#define NAMEMANAGER_H

#include <QStringList>



class NameManager
{
public:
  NameManager();

private:
  QStringList words_;//! Список наименований. Служит для хранения повторяющихся слов
};

#endif // NAMEMANAGER_H
