#include "menu.h"

Action::Action(QObject *parent) : QAction(parent)
{}

//

Menu::Menu(QWidget *parent) : QMenu(parent)
{

}

QObject *Menu::createAction(const QString &name, QObject *parent)
{
  auto pact = new Action(parent);
  pact->setText(name);
  if(pact)
    pact->setObjectName(name);
  QMenu::addAction(pact);
  return(pact);
}

QObject *Menu::getAction(const QString &name)
{
  return findChild<QAction*>(name);
}

void Menu::clear()
{
  QMenu::clear();
}

QObject *Menu::addMenu(const QString &title)
{
  auto subMenu = new Menu(this);
  subMenu->setTitle(title);
  QMenu::addMenu(subMenu);
  return subMenu;
}

QObject *Menu::addSeparator()
{
  return QMenu::addSeparator();
}
