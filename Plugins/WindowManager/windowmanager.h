#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "windowmanager_global.h"
#include <Interfaces/iwindowmanager.h>

#include <QObject>

class QWidget;
class QMainWindow;

class WINDOWMANAGERSHARED_EXPORT WindowManager : public QObject, public IWindowManager
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID IWindowManager_iid)
  Q_INTERFACES(IWindowManager)
  //Q_PROPERTY(QMainWindow* mainWindow READ main_window)

public:

  explicit WindowManager(QObject *parent = 0);

  QMainWindow *main_window() const;
  void setMain_window(QMainWindow *main_window);

  // IWindowManager interface
public:
  int getVersion() override;

signals:

public slots:
  QWidget * createMainWindow(QWidget *parent = 0);
  QObject * createLayout();
  QObject * createWidget(QWidget *parent = 0);
  QObject * createLabel(QWidget *parent = 0);
  QObject * createLineEdit(QWidget *parent = 0);
  QObject * createTextEdit(QWidget *parent = 0);
  QObject * createPlainTextEdit(QWidget *parent = 0);
  QObject * createPushButton(QWidget *parent = 0);
  QObject * createListView(QWidget *parent = 0);
  QObject * createTreeView(QWidget *parent = 0);
  QObject * createTableView(QWidget *parent = 0);
  QObject * createDialog(QWidget *parent = 0);
  QObject * createGraphicsView(QWidget *parent = 0);
  QObject * createToolBar(QWidget *parent = 0);
  QObject * createFileDialog(QWidget *parent = 0);
  QObject * createMenu(QWidget *parent = 0);
  QWidget * createProgressBar(QWidget *parent = 0);

protected:
  void makeParent(QWidget *&parent);

private:
  QMainWindow *main_window_ =0;

};

#endif // WINDOWMANAGER_H
