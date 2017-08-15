#ifndef INTELLECTPLUGIN_H
#define INTELLECTPLUGIN_H

class QScriptEngine;
class QJSEngine;

class IntellectPlugin
{
public:
  explicit IntellectPlugin(QScriptEngine *engine);
};

class QJSIntellectPlugin
{
public:
  explicit QJSIntellectPlugin(QJSEngine *engine);
};

#endif // INTELLECTPLUGIN_H
