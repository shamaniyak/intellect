#include "intellect.h"
#include "GUI/mainwindowex.h"
#include <QApplication>
#include <Test/TestSppzCartography/testsppzcartography.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //TestSppzCartography test;

    // ������� ������
    // ������ ��� ����� ��� ����, ����� ��� ����������� �������, �� ������������ ������
    Intellect *pI = new Intellect(&a);

    // ������� ������� ���� ���������
    MainWindowEx *w = new MainWindowEx(pI);
    w->showMaximized();

    emit pI->start();// ��������� ������

    int result = a.exec();

    pI->stop();

    delete w;// ������������� ������ ��������� ������ ������

    return result;
}
