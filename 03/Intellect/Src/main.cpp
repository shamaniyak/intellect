#include "intellect.h"
#include "GUI/mainwindowex.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // ������� ������
    // ������ ��� ����� ��� ����, ����� ��� ����������� �������, �� ������������ ������
    Intellect *pI = new Intellect(&a);

    // ������� ������� ���� ���������
    MainWindowEx *w = new MainWindowEx(pI);
    w->showMaximized();

    pI->start();// ��������� ������

    int result = a.exec();

    pI->stop();

    delete w;// ������������� ������ ��������� ������ ������

    return result;
}
