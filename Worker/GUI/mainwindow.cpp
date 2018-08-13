#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scriptwrapper.h"
#include <QtWidgets>
#include "formMemoryEditor.h"
#include <tmemory.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->txtLog->clear();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::addOtvet(const QString &txt)
{
  if(txt.isEmpty())
    return;

  QString str = ui->txtLog->toPlainText();
  if(!str.isEmpty())
    str += "\n";
  str += txt;
  ui->txtLog->setPlainText(str);
}

void MainWindow::on_btnCompile_clicked()
{
  //

  ui->txtLog->clear();

  ScriptWrapper script;
  script.addObject(ui->txtLog);
  script.setText(ui->plainTextEdit->toPlainText());

  if(!script.compile())
    addOtvet("Compile error: ");

  addOtvet(script.msg());
}

void MainWindow::on_btnExec_clicked()
{
  QLabel lbl;
  //
  ScriptWrapper script;
  script.addObject(&lbl, "lbl");
  script.addObject(ui->txtLog);
  script.setText(ui->plainTextEdit->toPlainText());
  if(!script.execute())
    addOtvet("Evaluate error: ");

  addOtvet(script.msg());
}

void MainWindow::on_btnTest_clicked()
{

}

void MainWindow::on_btnMemory_clicked()
{

}
