#include "Src/worker.h"
#include "GUI/workermainwindowex.h"
#include <QApplication>

class Application : public QApplication
{
public:
  Application(int &argc, char **argv) : QApplication(argc, argv)
  {
    // ������� ������
    // ������ ��� ����� ��� ����, ����� ��� ����������� �������, �� ������������ ������
    worker_ = new Worker(this);

    // ������� ������� ���� ���������
    w = new WorkerMainWindowEx(worker_);
    w->showMaximized();

    emit worker_->start();// ��������� ������
  }

  ~Application()
  {
    worker_->stop();

    delete w;// ������������� ������ ��������� ������ ������

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
