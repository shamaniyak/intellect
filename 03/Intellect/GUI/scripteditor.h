#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

#include <QObject>
#include <QPlainTextEdit>
#include <memorywrapper.h>

class Highlighter;
class Intellect;

class ScriptEditor : public QPlainTextEdit
{
  Q_OBJECT

public:
  ScriptEditor(QWidget *parent = 0);

  MemoryWrapper *mem() const;
  void setMem(MemoryWrapper *mem);

  Intellect *intellect() const;
  void setIntellect(Intellect *intellect);

signals:
  signalKeyPress(QObject *obj, int key);

protected:
  void timerEvent(QTimerEvent *);
  void keyPressEvent(QKeyEvent * kev);
  void checkForSave();
  void save();
  void doReturn();

protected slots:
  void memory_change(MEWrapper *me, EMemoryChange idMsg);
  void on_textChanged();

private:
  Highlighter *h_ = 0;
  Intellect *intellect_ = 0;
  MemoryWrapper *mem_ = 0;
  MEWrapper *sel_ = 0;
  int timerId_ = 0;

  void showVal();
  void disconnectMem();
  void connectMem();
};

#endif // SCRIPTEDITOR_H
