#ifndef IOBJECT_H
#define IOBJECT_H

#include <QObject>
#include <memory>

#include "memorywrapper.h"
#include "scriptwrapper.h"
#include "qqmlwrapper.h"
#include "commander.h"

class WindowManager;
class ObjectManager;

class IObject : public QObject
{
  Q_OBJECT
  Q_PROPERTY(MemoryWrapper* mem READ mem)
  Q_PROPERTY(WindowManager* WM READ getWM)
  //Q_PROPERTY(QObject* cm READ CM)
  Q_PROPERTY(QObject* OM READ getOM)
  //Q_PROPERTY(QString msg READ msg)
public:
  typedef std::shared_ptr<JSEngineWrapper> t_jswrapper;
  typedef std::shared_ptr<QQmlEngineWrapper> t_qmlwrapper;

  explicit IObject(QObject *parent = 0);
  ~IObject();

  t_jswrapper scr() const;

  //QString msg() const;

  int countLinks() const;
  void incCountLinks();

  QObject *getOM();

signals:
  // Послать запрос объекту с именем receiver.
  // me - входные и выходные параметры
  // path - путь к элементу памяти, где находится скрипт
  // params - это параметры в формате командной страки (может быть скриптом)
  signalQuery(const QString &path, const QString &params);

public slots:
  void release();

  MemoryWrapper *mem();
  // Выполнить скрипт, записанный в памяти объекта, находящийся по пути path
  bool run(const QString &path, const QVariant &params=QVariant());

  // Выполнить скрипт
  bool execute(const QString &text);

  void reset();

  // Выполнить QML код
  bool runQML(const QString &text);

  // Добавить объект в скрипт.
  // name - имя объекта, по которому можно обращаться в скрипте.
  // Если name не задано, то у объекта должно быть задано имя objectName, иначе не добавится.
  // Если объект с таким именем существует, то не добавится.
  bool addObject(QObject *obj, const QString &name);
  // Создает новый или возвращает существующий объект
  QObject *getObject(const QString &name);

  //

  // Добавить сообщение
  void add_msg(const QString &msg);

  // Оконный мэнеджер
  // Если объект не создан, то создать. Вернуть объект
  WindowManager *getWM();

  //
  QObject *CM();

  // Послать команду объекту с именем receiver.
  // me - входные и выходные параметры
  // path - путь к элементу памяти, где находится скрипт
  // params - это параметры в формате командной страки (может быть скриптом)
  void sendQuery(const QString &receiver, const QString &path, const QString &params);

protected slots:
  void onQuery(const QString &path, const QString &params);

private:
  int countLinks_ = 0;
  MemoryWrapper *mem_ = 0;// Данные объекта в виде дерева
  t_jswrapper scr_;// Функционал объекта в виде скрипта, расширяющего его возможности
  t_qmlwrapper qml_;
  QString msg_;// Сообщение об выполненных операциях
  WindowManager *WM_ = 0;// Оконный мэнеджер
  ObjectManager *OM_ = 0;
  Commander *CM_ = 0;// Мэнеджер команд
};

Q_DECLARE_METATYPE(IObject*)

#endif // IOBJECT_H
