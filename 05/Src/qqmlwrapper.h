#ifndef QQMLWRAPPER_H
#define QQMLWRAPPER_H

#include <QObject>
#include <QtQml>

//
class QmlEngineWrapper : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString msg READ msg)

public:
	explicit QmlEngineWrapper(QObject *parent=nullptr);
	~QmlEngineWrapper();

	Q_INVOKABLE bool evaluate(const QString &txt, const QString &path=QString());

  bool addObject(QObject *_o, const QString &_name=QString());
  QObject* getObject(const QString &name) const;

  static void init();

  QString msg() const;

  Q_INVOKABLE void reset();
  Q_INVOKABLE void addImportPath(const QString &path);
  Q_INVOKABLE void load(const QString &path);

protected:
  void insertObjectsInQml();

private:
  void createQmlEngine();

  QQmlEngine *m_qml = nullptr;
  QObject *m_tempObject = nullptr;
  QString m_msg;
  QMap<QString, QObject*> m_objects;

  static QQmlApplicationEngine *engine;

	QmlEngineWrapper(const QmlEngineWrapper &val);
	QmlEngineWrapper & operator =(const QmlEngineWrapper &);

};

#endif // QQMLWRAPPER_H
