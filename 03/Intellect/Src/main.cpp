#include "intellect.h"
#include "GUI/intellectmainwindowex.h"
#include <QApplication>
#include <Test/TestSppzCartography/testsppzcartography.h>

class Main
{
public:
  Main(QObject *parent)
  {
    // создать модель
    // парент нам нужен для того, чтобы при уничтожении скрипта, не уничтожалась модель
    pI = new Intellect(parent);

    // создать главное окно редактора
    w = new IntellectMainWindowEx(pI);
    w->showMaximized();

    emit pI->start();// запустить модель
  }

  ~Main()
  {
    pI->stop();

    delete w;// просмоторщики должны удалиться раньше модели

    delete pI;
  }

private:
  Intellect *pI = nullptr;
  IntellectMainWindowEx *w = nullptr;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Main main_(&a);

    int result = a.exec();

    return result;
}
