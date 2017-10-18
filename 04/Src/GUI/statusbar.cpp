#include "statusbar.h"

StatusBar::StatusBar(QWidget *parent) : QStatusBar(parent)
{

}

void StatusBar::addWidget(QWidget *widget, int stretch)
{
  QStatusBar::addWidget(widget, stretch);
}

void StatusBar::removeWidget(QWidget *widget)
{
  QStatusBar::removeWidget(widget);
}
