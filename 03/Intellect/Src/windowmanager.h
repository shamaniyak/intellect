#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>
#include <memory>

class QWidget;
class QMainWindow;
class MemoryWrapper;

class WindowManager : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QMainWindow* mainWindow READ main_window)

public:

  explicit WindowManager(QObject *parent = 0);

  QMainWindow *main_window() const;
  void setMain_window(QMainWindow *main_window);

signals:

public slots:
  QObject * createWidget(QWidget *parent = 0);
  QObject * createLabel(QWidget *parent = 0);
  QObject * createLineEdit(QWidget *parent = 0);
  QObject * createTextEdit(QWidget *parent = 0);
  QObject * createPlainTextEdit(QWidget *parent = 0);
  QObject * createPushButton(QWidget *parent = 0);
  QObject * createListView(QWidget *parent = 0);
  QObject * createTreeView(MemoryWrapper *mem, QWidget *parent = 0);
  QObject * createTableView(QWidget *parent = 0);
  QObject *createDialog(QWidget *parent = 0);
  QObject *createGraphicsView(QWidget *parent = 0);
  QObject *createToolBar(QWidget *parent = 0);
  QObject *createFileDialog(QWidget *parent = 0);

  QObject * createMemoryView(QWidget *parent = 0);
  QObject * createMemoryEditor(QWidget *parent = 0);

protected:
  void makeParent(QWidget *&parent);

private:
  std::shared_ptr<QWidget> top_widget_;
  QMainWindow *main_window_ =0;
};

#endif // WINDOWMANAGER_H
