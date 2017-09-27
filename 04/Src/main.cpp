#include "intellect.h"
#include "GUI/intellectmainwindowex.h"
#include <QApplication>
#include <Test/TestSppzCartography/testsppzcartography.h>

class Main
{
public:
  Main(QObject *parent)
  {
    // ������� ������
    // ������ ��� ����� ��� ����, ����� ��� ����������� �������, �� ������������ ������
    pI = new Intellect(parent);

    // ������� ������� ���� ���������
    w = new IntellectMainWindowEx(pI);
    w->showMaximized();

    emit pI->start();// ��������� ������
  }

  ~Main()
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
    QApplication a(argc, argv);

    Main main_(&a);

    int result = a.exec();

    return result;
}
