#ifndef QMLREGISTERTYPES_H
#define QMLREGISTERTYPES_H

#include <QObject>

class QmlRegisterTypes : public QObject
{
  Q_OBJECT
public:
  explicit QmlRegisterTypes(QObject *parent = nullptr);

  static void init();

signals:

public slots:
};

#endif // QMLREGISTERTYPES_H
