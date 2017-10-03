#ifndef MAINWINDOWEX_H
#define MAINWINDOWEX_H

#include <QMainWindow>
#include <QtWidgets>

namespace Ui {
class MainWindowEx;
}

class MainWindowEx : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindowEx(QWidget *parent = 0);
  ~MainWindowEx();

public slots:
  QWidget *createNewDoc(QWidget *wgt, bool deleteOnClose = true);
  QWidget *createNewDockWidget(QWidget *wgt, bool deleteOnClose = true);
  QWidget *createDocument(QWidget *wgt);
  QWidget *createDockWidget(QWidget *wgt);

  void addToolBar(QObject *tb);
  QObject *getToolBar(const QString &name);

  QWidget *getMenuBar();

  QWidget *getStatusBar();

  QObject *addMenu(const QString &name);
  void addMenu(QObject *menu);
  QObject *getMenu(const QString &name);

protected:

  virtual void closeEvent(QCloseEvent *event);

private:
  Ui::MainWindowEx *ui;
};

#endif // MAINWINDOWEX_H
