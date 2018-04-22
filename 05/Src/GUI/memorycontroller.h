#ifndef MEMORYCONTROLLER_H
#define MEMORYCONTROLLER_H

#include <QObject>

#include <Src/Memory/memorywrapper.h>

class IObject;
class QMemoryModel;
class QPlainTextEdit;


class MemoryController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(MemoryWrapper* mem READ mem WRITE setMem)
  Q_PROPERTY(MEWrapper me READ me WRITE setMe)
public:
  explicit MemoryController(QObject *parent = nullptr);

  MemoryWrapper *mem() const;
  void setMem(MemoryWrapper *mem);

  MEWrapper me();
  void setMe(const MEWrapper &me);

  IObject *iobj() const;
  void setIobj(IObject *iobj);

  bool canChangeSelected() const;
  void setCanChangeSelected(bool canChangeSelected);

  QPlainTextEdit *textEdit() const;
  void setTextEdit(QPlainTextEdit *textEdit);

signals:

public slots:

protected:
  void timerEvent(QTimerEvent *);

  void checkForSave();
  void doSave();
  void showVal();

protected slots:
  void memory_change(const MEWrapper &me, EMemoryChange idMsg);
  void on_textChanged();

private:
  QPlainTextEdit *textEdit_ = 0;
  int timerId_ = 0;
  IObject *iobj_ = 0;
  MemoryWrapper *mem_ = 0;
  MEWrapper sel_;
  bool canChangeSelected_ = true;

  void disconnectMem();
  void connectMem();
};

#endif // MEMORYCONTROLLER_H
