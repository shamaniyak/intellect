#ifndef QQMLWRAPPER_H
#define QQMLWRAPPER_H

#include <QObject>
#include <QtQml>

//
class QQmlEngineWrapper : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString msg READ msg)

public:
  explicit QQmlEngineWrapper(QObject *parent=nullptr);
  ~QQmlEngineWrapper();

  Q_INVOKABLE bool evaluate(const QString &txt);

  bool addObject(QObject *_o, const QString &_name=QString());
  QObject* getObject(const QString &name) const;

  static void init();

  QString msg() const;

  Q_INVOKABLE void reset();

protected:
  void insertObjectsInQml();

private:
  QQmlEngine *m_qml = nullptr;
  QObject *m_tempObject = nullptr;
  QString m_msg;
  QMap<QString, QObject*> m_objects;

  static QQmlApplicationEngine *engine;

  QQmlEngineWrapper(const QQmlEngineWrapper &val);
  QQmlEngineWrapper & operator =(const QQmlEngineWrapper &);

};

#endif // QQMLWRAPPER_H
