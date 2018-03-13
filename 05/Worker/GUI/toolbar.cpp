#include "toolbar.h"
#include <QAction>

ToolBar::ToolBar(QWidget *parent) : QToolBar(parent)
{

}

QObject *ToolBar::createAction(const QString &name)
{
  auto pact = this->addAction(name);
  if(pact)
    pact->setObjectName(name);
  return(pact);
}

QObject *ToolBar::getAction(const QString &name)
{
  return findChild<QAction*>(name);
}

void ToolBar::clear()
{
  QToolBar::clear();
}
