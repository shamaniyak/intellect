#include "Src/intellect.h"
#include "GUI/intellectmainwindowex.h"
#include <Test/TestSppzCartography/testsppzcartography.h>

#include <QApplication>
#include <QQmlApplicationEngine>

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

//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QLatin1String("qrc:/Src/Quick/main.qml")));
//    if (engine.rootObjects().isEmpty())
//      return -1;

    int result = a.exec();

    return result;
}
