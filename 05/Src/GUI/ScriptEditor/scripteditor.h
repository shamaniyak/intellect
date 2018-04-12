#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

#include <QObject>
#include <QPlainTextEdit>
#include <QCompleter>

#include <Src/Memory/memorywrapper.h>

class QSyntaxHighlighter;
class IObject;
class Completer;
class QMemoryModel;

class ScriptEditor : public QPlainTextEdit
{
  Q_OBJECT
  Q_PROPERTY(MemoryWrapper* mem READ mem WRITE setMem)
  Q_PROPERTY(MEWrapper me READ me WRITE setMe)

public:
  explicit ScriptEditor(QWidget *parent = 0);
  ~ScriptEditor();

  MemoryWrapper *mem() const;
  void setMem(MemoryWrapper *mem);

  MEWrapper me();
  void setMe(const MEWrapper &me);

  void lineNumberAreaPaintEvent(QPaintEvent *event);
  int lineNumberAreaWidth();

  IObject *iobj() const;
  void setIobj(IObject *iobj);

  bool getCanChangeSelected() const;
  void setCanChangeSelected(bool canChangeSelected);

signals:
  void signalKeyPress(QObject *obj, int key);

protected:
  void timerEvent(QTimerEvent *);
  void keyPressEvent(QKeyEvent * kev);
  void resizeEvent(QResizeEvent *event);
  void mousePressEvent(QMouseEvent *event);

  void checkForSave();
  void save();
  void doReturn();

protected slots:
  void memory_change(const MEWrapper &me, EMemoryChange idMsg);
  void on_textChanged();

private slots:
  void updateLineNumberAreaWidth(int newBlockCount);
  void highlightCurrentLine();
  void updateLineNumberArea(const QRect &rect, int dy);
  void insertCompletion(const QString&completion);
  void performCompletion();

private:
  QSyntaxHighlighter *h_ = 0;
  IObject *iobj_ = 0;
  MemoryWrapper *mem_ = 0;
  MEWrapper sel_;
  int timerId_ = 0;
  QWidget *lineNumberArea_ = 0;
  Completer *completer = 0;
  bool canChangeSelected_ = false;

  void showVal();
  void disconnectMem();
  void connectMem();
  void createHighliter();
  void createLineNumberArea();
  void init();
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
  bool keyPressEvent(QKeyEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void insertCompletion(const QString &completion, bool singleWord);

protected:
  void performCompletion(const QString &completionPrefix);
  void populateModel(const QString &completionPrefix);
  bool handledCompletedAndSelected(QKeyEvent *event);

private:
  bool completedAndSelected = false;
  QStringListModel *model = nullptr;
  QPlainTextEdit *editor = nullptr;
};

#endif // SCRIPTEDITOR_H
