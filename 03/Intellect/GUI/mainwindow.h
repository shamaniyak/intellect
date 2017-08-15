#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tmemory.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
  void addOtvet(const QString &txt);

private slots:
  void on_btnCompile_clicked();

  void on_btnExec_clicked();

  void on_btnTest_clicked();

  void on_btnMemory_clicked();

private:
    Ui::MainWindow *ui;
    Memory::TMemory M;
};

#endif // MAINWINDOW_H
