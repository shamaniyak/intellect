#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>

class StatusBar : public QStatusBar
{
  Q_OBJECT
public:
  explicit StatusBar(QWidget *parent = nullptr);

signals:

public slots:
  void addWidget(QWidget *widget, int stretch = 0);
  void removeWidget(QWidget *widget);
};

#endif // STATUSBAR_H
