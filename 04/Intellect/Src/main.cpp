#include "Src/intellect.h"
#include "GUI/intellectmainwindowex.h"
#include <QApplication>
#include <Test/TestSppzCartography/testsppzcartography.h>

class Application : public QApplication
{
public:
  Application(int &argc, char **argv) : QApplication(argc, argv)
  {
    // создать модель
    // парент нам нужен для того, чтобы при уничтожении скрипта, не уничтожалась модель
    pI = new Intellect(this);

    // создать главное окно редактора
    w = new IntellectMainWindowEx(pI);
    w->showMaximized();

    emit pI->start();// запустить модель
  }

  ~Application()
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
    Application a(argc, argv);

    int result = a.exec();

    return result;
}
