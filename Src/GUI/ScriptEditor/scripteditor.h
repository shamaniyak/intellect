#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

#include <QObject>
#include <QPlainTextEdit>
#include <QCompleter>

class QSyntaxHighlighter;
class Completer;

class ScriptEditor : public QPlainTextEdit
{
  Q_OBJECT

public:
  explicit ScriptEditor(QWidget *parent = 0);
  ~ScriptEditor();

  void lineNumberAreaPaintEvent(QPaintEvent *event);
  int lineNumberAreaWidth();

signals:
  void signalKeyPress(QObject *obj, int key, bool ctrl, bool shift, bool alt);

protected:
  void keyPressEvent(QKeyEvent * kev);
  void resizeEvent(QResizeEvent *event);
  void mousePressEvent(QMouseEvent *event);

  void doReturn(bool ctrl);
  bool doTab(bool shift);
  void doComment();

protected slots:

private slots:
  void updateLineNumberAreaWidth(int newBlockCount);
  void highlightCurrentLine();
  void updateLineNumberArea(const QRect &rect, int dy);
  void insertCompletion(const QString&completion);
  void performCompletion();

private:
  QSyntaxHighlighter *h_ = 0;
  QWidget *lineNumberArea_ = 0;
  Completer *completer = 0;

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
