#ifndef SCRIPTWRAPPER_H
#define SCRIPTWRAPPER_H

#include <QtCore>
#include <QtScript>
#include <QJSEngine>
#include <thread>

class QScriptEngineDebugger;

class ScriptWrapper
{
  struct Exec
  {
    Exec(ScriptWrapper *scr) : scr_(scr)
    {

    }



  private:
    ScriptWrapper *scr_ = 0;
  };

public:
  explicit ScriptWrapper(ScriptWrapper *parent=nullptr);
  virtual ~ScriptWrapper();

  ScriptWrapper(const ScriptWrapper &val);
  ScriptWrapper & operator =(const ScriptWrapper &);

  void clear();

  QString text() const;
  void setText(const QString &text);

  QScriptEngine *script();

  virtual void abort();

  virtual bool compile();
  virtual bool execute();

  QString msg() const;
  void setMsg(const QString &msg);

  virtual bool addObject(QObject *_o, const QString &_name=QString());

  QObject* getObject(const QString &name) const;

protected:
  void add_objects(const ScriptWrapper* parent);

  void insertObjectsInScript();

private:
  ScriptWrapper *m_parent = nullptr;
  QScriptEngine *m_script = nullptr;
  QScriptEngineDebugger *m_debugger = nullptr;
  QMap<QString, QObject*> m_objects;

  QString m_text;
  bool textChanged = false;

  QString m_msg;

  void deleteScript();
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

  virtual bool execute() override;

  bool addObject(QObject *_o, const QString &_name=QString()) override;

private:
  QJSEngine m_jsengine;
  //std::thread m_thr;
  bool m_debug = false;

  void deleteJSengine();
  void initPlugins();
};



#endif // SCRIPTWRAPPER_H
