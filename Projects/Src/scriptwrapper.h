#ifndef SCRIPTWRAPPER_H
#define SCRIPTWRAPPER_H

#include <QObject>
#include <QJSEngine>

class ScriptWrapper : public QObject
{

public:
  explicit ScriptWrapper(QObject *parent=nullptr);
  virtual ~ScriptWrapper();

  ScriptWrapper(const ScriptWrapper &val);
  ScriptWrapper & operator =(const ScriptWrapper &);

  void clear();

  virtual void abort();

  virtual bool evaluate(const QString &txt);

  QString msg() const;
  void setMsg(const QString &msg);

  virtual bool addObject(QObject *_o, const QString &_name=QString());

  QObject* getObject(const QString &name) const;

protected:

  void add_objects(const ScriptWrapper* parent);

  void insertObjectsInScript();

  QMap<QString, QObject*> m_objects;

private:
  ScriptWrapper *m_parent = nullptr;
  //QScriptEngine *m_script = nullptr;
  //QScriptEngineDebugger *m_debugger = nullptr;

  QString m_msg;

  void initPlugins();
};

//
class JSEngineWrapper : public ScriptWrapper
{
public:
  explicit JSEngineWrapper(ScriptWrapper *parent=nullptr);
  ~JSEngineWrapper();

  JSEngineWrapper(const JSEngineWrapper &val);
  JSEngineWrapper & operator =(const JSEngineWrapper &);

  bool addObject(QObject *_o, const QString &_name=QString()) override;

  virtual bool evaluate(const QString &txt);

protected:

private:
  QJSEngine m_jsengine;

  void deleteJSengine();
  void initPlugins();
  void insertObjectsInScript();
};



#endif // SCRIPTWRAPPER_H
