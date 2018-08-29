#include "Src/intellect.h"
#include "GUI/intellectmainwindowex.h"
#include "inputpassword.h"
#include <Src/qqmlwrapper.h>

#include <QApplication>
#include <QQmlApplicationEngine>

class Application : public QApplication
{
public:
  Application(int &argc, char **argv) : QApplication(argc, argv)
  {
		QmlEngineWrapper::init();
    // ������� ������
    // ������ ��� ����� ��� ����, ����� ��� ����������� �������, �� ������������ ������
    pI = new Intellect(this);

    if(inputPassword())
    {
      // ������� ������� ���� ���������
      w = new IntellectMainWindowEx(pI);
      w->show();

      emit pI->start();// ��������� ������

      running_ = true;
    }

  }

  ~Application()
  {
    if(pI) pI->stop();

    if(w) delete w;// gui ������ ��������� ������ ������

    if(pI) delete pI;
  }

  bool running() const
  {
    return running_;
  }

protected:
  bool inputPassword()
  {
    auto me = pI->obj()->mem()->get("Intellect/password");
    if(!me || me.val().toString().isEmpty())
      return true;

    InputPassword ip;
    ip.exec();
    return ip.isTrue(me.val().toString());
  }

private:
  Intellect *pI = nullptr;
  IntellectMainWindowEx *w = nullptr;
  bool running_ = false;
};

int main(int argc, char *argv[])
{
		Application a(argc, argv);

//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QLatin1String("qrc:/mainqml")));
//    if (engine.rootObjects().isEmpty())
//      return -1;

    if(!a.running())
      return -123;

    return a.exec();
}


