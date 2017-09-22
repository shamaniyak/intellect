#ifndef MENU_H
#define MENU_H

#include <QMenu>

class Menu : public QMenu
{
  Q_OBJECT
public:
  explicit Menu(QWidget *parent = 0);

signals:

public slots:
  QObject *createAction(const QString &name);
  QObject *getAction(const QString &name);
  void clear();

protected:

};

#endif // MENU_H
