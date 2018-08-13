#ifndef MEMORYPLUGIN_H
#define MEMORYPLUGIN_H

class QScriptEngine;
class QJSEngine;

class MemoryPlugin
{

public:
  explicit MemoryPlugin(QScriptEngine *engine);
  ~MemoryPlugin() {}
};


class QJSEngineMemoryPlugin
{

public:
  explicit QJSEngineMemoryPlugin(QJSEngine &engine);
  ~QJSEngineMemoryPlugin() {}

  static void registerTypes();
};


//class QmlMemoryPlugin : public QQmlExtensionPlugin
//{
//    Q_OBJECT
//    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

//public:
//  void registerTypes(const char *uri)
//  {
//    Q_ASSERT(uri == QLatin1String("MemoryWrapper"));
//    qmlRegisterType<MemoryWrapper>(uri, 1, 0, "Time");
//  }
//};

//class QExampleQmlPlugin : public QQmlExtensionPlugin
//{
//    Q_OBJECT
//    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

//public:
//    void registerTypes(const char *uri)
//    {
//        Q_ASSERT(uri == QLatin1String("TimeExample"));
//        qmlRegisterType<TimeModel>(uri, 1, 0, "Time");
//    }
//};

#endif // MEMORYPLUGIN_H
