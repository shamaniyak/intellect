#include "Src/worker.h"
#include "GUI/workermainwindowex.h"
#include <QApplication>

class Application : public QApplication
{
public:
  Application(int &argc, char **argv) : QApplication(argc, argv)
  {
    // создать модель
    // парент нам нужен для того, чтобы при уничтожении скрипта, не уничтожалась модель
    worker_ = new Worker(this);

    // создать главное окно редактора
    w = new WorkerMainWindowEx(worker_);
    w->showMaximized();

    emit worker_->start();// запустить модель
  }

  ~Application()
  {
    worker_->stop();

    delete w;// просмоторщики должны удалиться раньше модели

    delete worker_;
  }

private:
  Worker *worker_ = nullptr;
  WorkerMainWindowEx *w = nullptr;
};

int main(int argc, char *argv[])
{
    Application a(argc, argv);

    int result = a.exec();

    return result;
}
