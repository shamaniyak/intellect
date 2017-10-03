#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QObject>
#include <QToolBar>

class ToolBar : public QToolBar
{
  Q_OBJECT
public:
  explicit ToolBar(QWidget *parent = 0);

signals:

public slots:
  QObject *createAction(const QString &name);
  QObject *getAction(const QString &name);
  void clear();
};

#endif // TOOLBAR_H
