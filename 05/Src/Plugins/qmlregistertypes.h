#ifndef QMLREGISTERTYPES_H
#define QMLREGISTERTYPES_H

#include <QObject>

class QJSEngine;

class QmlRegisterTypes : public QObject
{
  Q_OBJECT
public:
  explicit QmlRegisterTypes(QObject *parent = nullptr);

  static void init();
	static void initJSEngine(QJSEngine*jse);

signals:

public slots:
};

#endif // QMLREGISTERTYPES_H
