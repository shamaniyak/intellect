#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

#include <QObject>
#include <QPlainTextEdit>
#include <QCompleter>

#include <Src/Memory/memorywrapper.h>

class Highlighter;
class IObject;
class Completer;
class QMemoryModel;

class ScriptEditor : public QPlainTextEdit
{
  Q_OBJECT

public:
  explicit ScriptEditor(QWidget *parent = 0);
  ~ScriptEditor();

  MemoryWrapper *mem() const;
  void setMem(MemoryWrapper *mem);

  void lineNumberAreaPaintEvent(QPaintEvent *event);
  int lineNumberAreaWidth();

  IObject *iobj() const;
  void setIobj(IObject *iobj);

signals:
  signalKeyPress(QObject *obj, int key);

protected:
  void timerEvent(QTimerEvent *);
  void keyPressEvent(QKeyEvent * kev);
  void resizeEvent(QResizeEvent *event);
  void mousePressEvent(QMouseEvent *event);

  void checkForSave();
  void save();
  void doReturn();

protected slots:
  void memory_change(MEWrapper *me, EMemoryChange idMsg);
  void on_textChanged();

private slots:
  void updateLineNumberAreaWidth(int newBlockCount);
  void highlightCurrentLine();
  void updateLineNumberArea(const QRect &rect, int dy);
  void insertCompletion(const QString&);
  void performCompletion();

private:
  Highlighter *h_ = 0;
  IObject *iobj_ = 0;
  MemoryWrapper *mem_ = 0;
  MEWrapper *sel_ = 0;
  int timerId_ = 0;
  QWidget *lineNumberArea_ = 0;
  Completer *completer = 0;

  void showVal();
  void disconnectMem();
  void connectMem();
};

class LineNumberArea : public QWidget
{
public:
  LineNumberArea(ScriptEditor *editor) : QWidget(editor) {
    codeEditor = editor;
  }

  QSize sizeHint() const {
    return QSize(codeEditor->lineNumberAreaWidth(), 0);
  }

protected:
  void paintEvent(QPaintEvent *event) {
    codeEditor->lineNumberAreaPaintEvent(event);
  }

private:
  ScriptEditor *codeEditor;
};


class QStringListModel;

class Completer : public QCompleter
{
public:
  Completer(QPlainTextEdit *parent = nullptr);

public:
  void performCompletion();
  void keyPressEvent(QKeyEvent *event);
  void mousePressEvent(QMouseEvent *event);

protected:
  void performCompletion(const QString &completionPrefix);
  void populateModel(const QString &completionPrefix);
  void insertCompletion(const QString &completion, bool singleWord);
  bool handledCompletedAndSelected(QKeyEvent *event);

private:
  bool completedAndSelected = false;
  QStringListModel *model = nullptr;
  QPlainTextEdit *editor = nullptr;
};

#endif // SCRIPTEDITOR_H
