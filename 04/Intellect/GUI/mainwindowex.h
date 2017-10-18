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
  Q_PROPERTY(QWidget* statusBar READ getStatusBar)
  Q_PROPERTY(QWidget* menuBar READ getMenuBar)

public:
  explicit MainWindowEx(QWidget *parent = 0);
  ~MainWindowEx();

public slots:
  QWidget *createNewDoc(QWidget *wgt, bool deleteOnClose = true);
  QWidget *createDocument(QWidget *wgt);

  QWidget *createNewDockWidget(QWidget *wgt, bool deleteOnClose = true);
  QWidget *createDockWidget(QWidget *wgt, int area = 0x1, int orientation = 0x1);
  void tabifyDockWidget(QDockWidget *first, QDockWidget *second);

  void addToolBar(QObject *tb);
  QWidget *getToolBar(const QString &name);

  QWidget *getMenuBar();

  QWidget *getStatusBar();

  QObject *createMenu(const QString &title);
  void addMenu(QObject *menu);
  QObject *getMenu(const QString &name);
  void deleteMenu(QObject *menu);

protected:

  virtual void closeEvent(QCloseEvent *event);

private:
  Ui::MainWindowEx *ui;
};

#endif // MAINWINDOWEX_H
