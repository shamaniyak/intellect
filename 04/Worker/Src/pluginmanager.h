#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>

class PluginManager : public QObject
{
  Q_OBJECT
public:
  explicit PluginManager(QObject *parent = 0);
  ~PluginManager();

  void clear();

  void loadPlugins();

  void addPlugin(QObject *pobj);

signals:

public slots:
  int count() const;
  QObject *get(unsigned int i) const;
  QObject *getByName(const QString &name);

private:
  typedef std::vector<QObject*> Plugins;
  Plugins plugins_;
};

#endif // PLUGINMANAGER_H
