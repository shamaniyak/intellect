#include "contextmenu.h"

ContextMenu::ContextMenu(QWidget *parent) : Menu(parent)
{

}

ContextMenu::~ContextMenu()
{

}

void ContextMenu::clear()
{
  QMenu::clear();
}

QObject *ContextMenu::addAction(const QString &text)
{
  return Menu::createAction(text);
}

QObject *ContextMenu::addSeparator()
{
  return QMenu::addSeparator();
}

