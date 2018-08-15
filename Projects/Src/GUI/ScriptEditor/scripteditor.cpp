#include "scripteditor.h"
#include "highlighter.h"
#include "SyntaxHighlighter.h"

//#include <Src/iobject.h>
//#include <Src/Memory/qmemorymodel.h>

#include <QDebug>
#include <QMessageBox>
#include <QPainter>
#include <QShortcut>
#include <QtAlgorithms>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QScrollBar>

ScriptEditor::ScriptEditor(QWidget *parent) : QPlainTextEdit(parent)
{
  init();

  createHighliter();

  createLineNumberArea();
}

ScriptEditor::~ScriptEditor()
{
  //qDebug() << "ScriptEditor::~ScriptEditor()";
}

void ScriptEditor::init()
{
  this->setObjectName("ScriptEditor");

  QFont font;
  font.setFamily("Courier");
  font.setFixedPitch(true);
  font.setPointSize(10);

  setFont(font);
  setLineWrapMode(QPlainTextEdit::NoWrap);
  setTabStopWidth(fontMetrics().width("  "));

  setPlaceholderText("Input value here");
}

void ScriptEditor::createHighliter()
{
  auto syntaxHighlighter = new SyntaxHighlighter(this);
  syntaxHighlighter->setObjectName("syntaxHighlighter");
  syntaxHighlighter->setDocument(document());
}

void ScriptEditor::createLineNumberArea()
{
  lineNumberArea_ = new LineNumberArea(this);

  completer = new Completer(this);

  connect(this, SIGNAL(blockCountChanged(int)),
          this, SLOT(updateLineNumberAreaWidth(int)));
  connect(this, SIGNAL(updateRequest(QRect,int)),
          this, SLOT(updateLineNumberArea(QRect,int)));
  connect(this, SIGNAL(cursorPositionChanged()),
          this, SLOT(highlightCurrentLine()));
  connect(completer, SIGNAL(activated(const QString&)),
          this, SLOT(insertCompletion(const QString&)));
  new QShortcut(QKeySequence(tr("Ctrl+Space", "Complete")),
                this, SLOT(performCompletion()));

  updateLineNumberAreaWidth(0);
  highlightCurrentLine();
}

void ScriptEditor::keyPressEvent(QKeyEvent *kev)
{
  bool ctrl_pressed = kev->modifiers() & Qt::ControlModifier;
  bool shift_pressed = kev->modifiers() & Qt::ShiftModifier;
  bool alt_pressed = kev->modifiers() & Qt::AltModifier;

  //qDebug() << "ctrl" << ctrl_pressed << "shift" << shift_pressed<< "alt" << alt_pressed << "key" << kev->key();
  //qDebug() << kev->text();

//  if(iobj_)
//  {
//    auto me = iobj_->mem()->add(MEWrapper(), "KeyEvent");
//    me.clear();

//    if(ctrl_pressed)
//      me.add("Ctrl");
//    if(shift_pressed)
//      me.add("Shift");
//    if(alt_pressed)
//      me.add("Alt");

//    me.add("Key").setVal(kev->key());
//    me.add("Text").setVal(kev->text());
//  }

  emit signalKeyPress(this, kev->key(), ctrl_pressed, shift_pressed, alt_pressed);

  if(completer->keyPressEvent(kev) )
    return;

  switch(kev->key())
  {
    case Qt::Key_Return:
    {
      doReturn(ctrl_pressed);
      kev->accept();
      return;
    }
    case Qt::Key_Tab:
    case Qt::Key_Backtab:
    {
      if(doTab(shift_pressed)) {
        kev->accept();
        return;
      }
    }
  case Qt::Key_Slash:
  {
    if(ctrl_pressed)
      doComment();
  }
  }

  QPlainTextEdit::keyPressEvent(kev);
}

void ScriptEditor::resizeEvent(QResizeEvent *e)
{
  QPlainTextEdit::resizeEvent(e);

  QRect cr = contentsRect();
  lineNumberArea_->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void ScriptEditor::mousePressEvent(QMouseEvent *event)
{
  completer->mousePressEvent(event);

  QPlainTextEdit::mousePressEvent(event);
}

void ScriptEditor::doReturn(bool ctrl)
{
  auto cursor = this->textCursor();
  cursor.beginEditBlock();
  if(ctrl) cursor.movePosition(QTextCursor::EndOfLine);
  QString text = cursor.block().text();
  int n = 0;
  while(text[n] == ' ' || text[n] == '\t') ++n;
  text = text.left(n);
  cursor.insertBlock();
  cursor.insertText(text);
  cursor.endEditBlock();
  this->setTextCursor(cursor);
}

bool ScriptEditor::doTab(bool shift)
{
  auto cursor = this->textCursor();
  if(cursor.selectedText().isEmpty()) return false;
  cursor.beginEditBlock();
  int selStart = cursor.selectionStart();
  int pos = cursor.position();
  auto lines = cursor.selectedText().split(QChar::ParagraphSeparator);
  for(int i = 0; i < lines.size(); ++i)
  {
    QString s = lines.at(i);
    if(shift) {
      if(s[0] == '\t' || s[0] == ' ')
        s.remove(0, 1);
    }
    else
      s = "\t" + s;
    lines[i] = s;
  }
  cursor.removeSelectedText();
  QString s = lines.join(QChar::ParagraphSeparator);
  cursor.insertText(s);
  if(pos > selStart) {
    cursor.setPosition(selStart);
    cursor.setPosition(selStart+s.length(), QTextCursor::KeepAnchor);
  } else {
    cursor.setPosition(selStart+s.length());
    cursor.setPosition(selStart, QTextCursor::KeepAnchor);
  }
  cursor.endEditBlock();
  this->setTextCursor(cursor);
  return true;
}

void ScriptEditor::doComment()
{
  auto cursor = this->textCursor();
  cursor.beginEditBlock();
  int selStart = cursor.selectionStart();
  int pos = cursor.position();
  auto lines = cursor.selectedText().split(QChar::ParagraphSeparator);
  for(int i = 0; i < lines.size(); ++i)
  {
    QString s = lines.at(i);
    if(s.length() >1 && s[0] == '/' && s[1] == '/')
       s.remove(0, 2);
    else
      s = "//" + s;
    lines[i] = s;
  }
  cursor.removeSelectedText();
  QString s = lines.join(QChar::ParagraphSeparator);
  cursor.insertText(s);
  if(pos > selStart) {
    cursor.setPosition(selStart);
    cursor.setPosition(selStart+s.length(), QTextCursor::KeepAnchor);
  } else {
    cursor.setPosition(selStart+s.length());
    cursor.setPosition(selStart, QTextCursor::KeepAnchor);
  }
  cursor.endEditBlock();
  this->setTextCursor(cursor);
}

void ScriptEditor::updateLineNumberAreaWidth(int /*newBlockCount*/)
{
  setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void ScriptEditor::highlightCurrentLine()
{
  QList<QTextEdit::ExtraSelection> extraSelections;

  if (!isReadOnly()) {
    QTextEdit::ExtraSelection selection;

    QColor lineColor = QColor(Qt::yellow).lighter(160);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);
  }

  setExtraSelections(extraSelections);
}

void ScriptEditor::updateLineNumberArea(const QRect &rect, int dy)
{
  if (dy)
    lineNumberArea_->scroll(0, dy);
  else
    lineNumberArea_->update(0, rect.y(), lineNumberArea_->width(), rect.height());

  if (rect.contains(viewport()->rect()))
    updateLineNumberAreaWidth(0);
}

void ScriptEditor::insertCompletion(const QString &completion)
{
  completer->insertCompletion(completion, false);
}

void ScriptEditor::performCompletion()
{
  completer->performCompletion();
}

void ScriptEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
  QPainter painter(lineNumberArea_);
  painter.fillRect(event->rect(), Qt::lightGray);
  //lineNumberAreaPaintEvent() вызываетс€ из LineNumberArea вс€кий раз при получении событи€ рисовани€. Ќачинаем рисование фона виджета.

  QTextBlock block = firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
  int bottom = top + (int) blockBoundingRect(block).height();
  //“еперь пройдЄмс€ по всем видимым строкам и отрисуем дл€ каждой строки номер в дополнительной области.
  //«аметьте, что при редактировании обычного текста кажда€ строка будет состо€ть из одного QTextBlock;
  //тем не менее, если включен перенос строк, то строка может занимать несколько строк в области просмотра редактора текста.

  //ѕолучим верхнюю и нижнюю y-координаты первого текстового блока,
  //и подгоним эти значени€ к высоте текущего текстового блока в каждой итерации цикла.

  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      QString number = QString::number(blockNumber + 1);
      painter.setPen(Qt::black);
      painter.drawText(0, top, lineNumberArea_->width(), fontMetrics().height(),
                       Qt::AlignRight, number);
    }

    block = block.next();
    top = bottom;
    bottom = top + (int) blockBoundingRect(block).height();
    ++blockNumber;
  }
}

int ScriptEditor::lineNumberAreaWidth()
{
  int digits = 1;
  int max = qMax(1, blockCount());
  while (max >= 10) {
    max /= 10;
    ++digits;
  }

  int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

  return space;
}

//  Completer

Completer::Completer(QPlainTextEdit *parent) : QCompleter(parent),
  editor(parent)
{
  model = new QStringListModel(this);

  setWidget(parent);
  setCompletionMode(QCompleter::PopupCompletion);
  setModel(model);
  setModelSorting(QCompleter::CaseInsensitivelySortedModel);
  setCaseSensitivity(Qt::CaseInsensitive);
  setWrapAround(true);
}

void Completer::performCompletion()
{
  QTextCursor cursor = editor->textCursor();
  cursor.select(QTextCursor::WordUnderCursor);
  const QString completionPrefix = cursor.selectedText();
  bool notEmpty = !completionPrefix.isEmpty();
  bool canPerform = notEmpty && completionPrefix.at(completionPrefix.length() - 1).isLetter();
  if (canPerform)
    performCompletion(completionPrefix);
}

void Completer::performCompletion(const QString &completionPrefix)
{
  populateModel(completionPrefix);

  if (completionPrefix != this->completionPrefix()) {
    this->setCompletionPrefix(completionPrefix);
    this->popup()->setCurrentIndex(this->completionModel()->index(0, 0));
  }

  if (completionCount() == 1)
    insertCompletion(currentCompletion(), true);
  else {
    QRect rect = editor->cursorRect();
    int newWidth = popup()->sizeHintForColumn(0) + popup()->verticalScrollBar()->sizeHint().width();
    rect.setWidth(newWidth);
    complete(rect);
  }
}

bool caseInsensitiveLessThan(const QString &a, const QString &b)
{
  return a.compare(b, Qt::CaseInsensitive) < 0;
}

void Completer::populateModel(const QString &completionPrefix)
{
  QStringList strings = editor->toPlainText().split(QRegExp("\\W+"));
  strings.removeAll(completionPrefix);
  strings.removeDuplicates();
  qSort(strings.begin(), strings.end(), caseInsensitiveLessThan);
  model->setStringList(strings);
}

void Completer::insertCompletion(const QString &completion, bool singleWord)
{
  QTextCursor cursor = editor->textCursor();
  int numberOfCharsToComplete = completion.length() -
      this->completionPrefix().length();
  int insertionPosition = cursor.position();
  cursor.insertText(completion.right(numberOfCharsToComplete));
  if (singleWord) {
    cursor.setPosition(insertionPosition);
    cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
    completedAndSelected = true;
  }
  editor->setTextCursor(cursor);
}

bool Completer::keyPressEvent(QKeyEvent *event)
{
  if (completedAndSelected && handledCompletedAndSelected(event))
    return false;
  completedAndSelected = false;
  if (popup()->isVisible()) {
    switch (event->key()) {
      case Qt::Key_Up: // ѕроваливаемс€
      case Qt::Key_Down: // ѕроваливаемс€
      case Qt::Key_Enter: // ѕроваливаемс€
      case Qt::Key_Return: // ѕроваливаемс€
      case Qt::Key_Escape: event->ignore(); return true;
      default: popup()->hide(); break;
    }
  }

  return false;
}

bool Completer::handledCompletedAndSelected(QKeyEvent *event)
{
  completedAndSelected = false;
  QTextCursor cursor = editor->textCursor();
  switch (event->key()) {
    case Qt::Key_Enter: // ѕроваливаемс€
    case Qt::Key_Return: cursor.clearSelection(); break;
    case Qt::Key_Escape: cursor.removeSelectedText(); break;
    default: return false;
  }
  editor->setTextCursor(cursor);
  event->accept();
  return true;
}

void Completer::mousePressEvent(QMouseEvent */*event*/)
{
  if (completedAndSelected) {
    completedAndSelected = false;
    QTextCursor cursor = editor->textCursor();
    cursor.removeSelectedText();
    editor->setTextCursor(cursor);
  }
}
