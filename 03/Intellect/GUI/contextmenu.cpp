#include "contextmenu.h"

ContextMenu::ContextMenu(QWidget *parent) : QMenu(parent)
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
  return QMenu::addAction(text);
}

QObject *ContextMenu::addSeparator()
{
  return QMenu::addSeparator();
}

