#include "Src/intellect.h"
#include "GUI/intellectmainwindowex.h"
#include <QApplication>
#include <Test/TestSppzCartography/testsppzcartography.h>

class Application : public QApplication
{
public:
  Application(int &argc, char **argv) : QApplication(argc, argv)
  {
    // ������� ������
    // ������ ��� ����� ��� ����, ����� ��� ����������� �������, �� ������������ ������
    pI = new Intellect(this);

    // ������� ������� ���� ���������
    w = new IntellectMainWindowEx(pI);
    w->showMaximized();

    emit pI->start();// ��������� ������
  }

  ~Application()
  {
    pI->stop();

    delete w;// ������������� ������ ��������� ������ ������

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
