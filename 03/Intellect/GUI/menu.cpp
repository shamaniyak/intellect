#include "menu.h"

Menu::Menu(QWidget *parent) : QMenu(parent)
{

}

QObject *Menu::createAction(const QString &name)
{
  auto pact = this->addAction(name);
  if(pact)
    pact->setObjectName(name);
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
