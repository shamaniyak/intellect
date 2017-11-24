#include "Src/intellect.h"
#include "GUI/intellectmainwindowex.h"
#include "inputpassword.h"

#include <QApplication>
#include <QQmlApplicationEngine>

class Application : public QApplication
{
public:
  Application(int &argc, char **argv) : QApplication(argc, argv)
  {
    if(inputPassword())
    {
      // создать модель
      // парент нам нужен для того, чтобы при уничтожении скрипта, не уничтожалась модель
      pI = new Intellect(this);

      // создать главное окно редактора
      w = new IntellectMainWindowEx(pI);
      w->show();

      emit pI->start();// запустить модель

      running_ = true;
    }

  }

  ~Application()
  {
    if(pI) pI->stop();

    if(w) delete w;// просмоторщики должны удалиться раньше модели

    if(pI) delete pI;
  }

  bool running() const
  {
    return running_;
  }

private:
  Intellect *pI = nullptr;
  IntellectMainWindowEx *w = nullptr;
  bool running_ = false;

  bool inputPassword()
  {
    //return true;
    InputPassword ip;
    ip.exec();
    return ip.isTrue();
  }
};

int main(int argc, char *argv[])
{
    Application a(argc, argv);

    QQmlApplicationEngine engine;
//    engine.load(QUrl(QLatin1String("qrc:/Src/Quick/main.qml")));
//    if (engine.rootObjects().isEmpty())
//      return -1;

    if(!a.running())
      return -123;

    return a.exec();
}


