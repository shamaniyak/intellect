#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>

class QDir;

class PluginManager : public QObject
{
  Q_OBJECT
public:
  explicit PluginManager(QObject *parent = 0);
  ~PluginManager();

  void clear();

  void loadPlugins();

signals:

public slots:
  int count() const;
  QObject *get(unsigned int i) const;
  QObject *getByName(const QString &name);
  QObject *loadPlugin(const QString &name);
  bool unloadPlugin(const QString &name);

private:
  typedef std::vector<QObject*> Plugins;
  Plugins plugins_;

  void addPlugin(QObject *pobj);
  QObject *loadPlugin(const QDir &dir, const QString &name);
};

#endif // PLUGINMANAGER_H
