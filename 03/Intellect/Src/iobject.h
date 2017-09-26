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
  // ������� ������ ������� � ������ receiver.
  // me - ������� � �������� ���������
  // path - ���� � �������� ������, ��� ��������� ������
  // params - ��� ��������� � ������� ��������� ������ (����� ���� ��������)
  signalQuery(const QString &path, const QString &params);

public slots:
  void release();

  MemoryWrapper *mem();
  // ��������� ������, ���������� � ������ �������, ����������� �� ���� path
  bool run(const QString &path, const QVariant &params=QVariant());

  // ��������� ������
  bool execute(const QString &text);

  void reset();

  // ��������� QML ���
  bool runQML(const QString &text);

  // �������� ������ � ������.
  // name - ��� �������, �� �������� ����� ���������� � �������.
  // ���� name �� ������, �� � ������� ������ ���� ������ ��� objectName, ����� �� ���������.
  // ���� ������ � ����� ������ ����������, �� �� ���������.
  bool addObject(QObject *obj, const QString &name);
  // ������� ����� ��� ���������� ������������ ������
  QObject *getObject(const QString &name);

  //

  // �������� ���������
  void add_msg(const QString &msg);

  // ������� ��������
  // ���� ������ �� ������, �� �������. ������� ������
  WindowManager *getWM();

  //
  QObject *CM();

  // ������� ������� ������� � ������ receiver.
  // me - ������� � �������� ���������
  // path - ���� � �������� ������, ��� ��������� ������
  // params - ��� ��������� � ������� ��������� ������ (����� ���� ��������)
  void sendQuery(const QString &receiver, const QString &path, const QString &params);

protected slots:
  void onQuery(const QString &path, const QString &params);

private:
  int countLinks_ = 0;
  MemoryWrapper *mem_ = 0;// ������ ������� � ���� ������
  t_jswrapper scr_;// ���������� ������� � ���� �������, ������������ ��� �����������
  t_qmlwrapper qml_;
  QString msg_;// ��������� �� ����������� ���������
  WindowManager *WM_ = 0;// ������� ��������
  ObjectManager *OM_ = 0;
  Commander *CM_ = 0;// �������� ������
};

Q_DECLARE_METATYPE(IObject*)

#endif // IOBJECT_H
