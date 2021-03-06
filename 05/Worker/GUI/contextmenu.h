#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

#include <QObject>
#include <QMenu>
#include "menu.h"

class ContextMenu : public Menu
{
  Q_OBJECT
public:
  ContextMenu(QWidget *parent = 0);
  ~ContextMenu();

public slots:
  void clear();
  QObject *addAction(const QString &text);
  QObject *addSeparator();
};

#endif // CONTEXTMENU_H
