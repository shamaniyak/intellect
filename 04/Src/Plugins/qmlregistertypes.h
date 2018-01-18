#ifndef QMLREGISTERTYPES_H
#define QMLREGISTERTYPES_H

#include <QObject>

class QmlRegisterTypes : public QObject
{
  Q_OBJECT
public:
  explicit QmlRegisterTypes(QObject *parent = nullptr);

signals:

public slots:
};

#endif // QMLREGISTERTYPES_H