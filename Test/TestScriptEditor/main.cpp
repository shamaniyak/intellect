#include "mainwindow.h"
#include <QApplication>

#include <Src/GUI/ScriptEditor/scripteditor.h>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  ScriptEditor se;
  se.show();
  //MainWindow w;
  //w.show();

  return a.exec();
}
