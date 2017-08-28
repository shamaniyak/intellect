#include "intellect.h"
#include "GUI/mainwindowex.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // создать модель
    // парент нам нужен для того, чтобы при уничтожении скрипта, не уничтожалась модель
    Intellect *pI = new Intellect(&a);

    // создать главное окно редактора
    MainWindowEx *w = new MainWindowEx(pI);
    w->showMaximized();

    pI->start();// запустить модель

    int result = a.exec();

    pI->stop();

    delete w;// просмоторщики должны удалиться раньше модели

    return result;
}
