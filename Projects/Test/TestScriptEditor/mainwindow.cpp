#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <Src/GUI/ScriptEditor/scripteditor.h>

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  ScriptEditor *e = new ScriptEditor;

  this->layout()->addWidget(e);
}

MainWindow::~MainWindow()
{
  delete ui;
}
