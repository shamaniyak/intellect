#ifndef QMLMANAGER_H
#define QMLMANAGER_H

#include <QObject>

class QmlManager : public QObject
{
  Q_OBJECT
public:
  explicit QmlManager(QObject *parent = nullptr);

signals:

public slots:
  QObject *createQmlEngine();
  QObject *createQmlComponent();
  QObject* createQuickView();
};

#endif // QMLMANAGER_H
