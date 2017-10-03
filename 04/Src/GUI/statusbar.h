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
};

#endif // STATUSBAR_H
