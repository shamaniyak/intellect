#ifndef MENU_H
#define MENU_H

#include <QMenu>

class Action : public QAction
{
  Q_OBJECT
  Q_PROPERTY(QObject* parent READ parent)

public:
  explicit Action(QObject *parent = 0);
};

class Menu : public QMenu
{
  Q_OBJECT

public:
  explicit Menu(QWidget *parent = 0);

signals:

public slots:
  QObject *createAction(const QString &name, QObject *parent = 0);
  QObject *getAction(const QString &name);
  void clear();

  QObject *addMenu(const QString &title);

  QObject *addSeparator();

protected:

};

#endif // MENU_H
