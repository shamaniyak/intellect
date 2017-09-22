#include "highlighter.h"
#include "scripteditor.h"

#include <intellect.h>
#include "qmemorymodel.h"

#include <QtDebug>
#include <QMessageBox>
#include <QPainter>
#include <QShortcut>
#include <QtAlgorithms>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QScrollBar>

ScriptEditor::ScriptEditor(QWidget *parent) : QPlainTextEdit(parent),
  h_(new Highlighter(this->document()))
{
  this->setObjectName("ScriptEditor");

  QFont font;
  font.setFamily("Courier");
  font.setFixedPitch(true);
  font.setPointSize(10);

  this->setFont(font);

  this->setLineWrapMode(QPlainTextEdit::NoWrap);

  setTabStopWidth(fontMetrics().width("  "));

  connect(this, &ScriptEditor::textChanged, this, &ScriptEditor::on_textChanged);

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

ScriptEditor::~ScriptEditor()
{
  qDebug() << "ScriptEditor::~ScriptEditor()";
}

MemoryWrapper *ScriptEditor::mem() const
{
  return mem_;
}

void ScriptEditor::disconnectMem()
{
  if(mem_)
    mem_->disconnect(this);
}

void ScriptEditor::connectMem()
{
  if(mem_)
    connect(mem_, &MemoryWrapper::on_change, this, &ScriptEditor::memory_change);
}

void ScriptEditor::setMem(MemoryWrapper *mem)
{
  disconnectMem();
  mem_ = mem;
  connectMem();
  if(mem_)
  {
    memory_change(mem_->getSelected(), mcSelect);
  }
}

void ScriptEditor::timerEvent(QTimerEvent *)
{
  save();
}

void ScriptEditor::keyPressEvent(QKeyEvent *kev)
{
  bool ctrl_pressed = kev->modifiers() & Qt::ControlModifier;
  bool shift_pressed = kev->modifiers() & Qt::ShiftModifier;
  bool alt_pressed = kev->modifiers() & Qt::AltModifier;

  if(intellect_)
  {
    IObject *editor = qobject_cast<IObject*>( intellect_->GetObject("Editor") );
    if(editor)
    {
      auto me = editor->mem()->add(0, "KeyEvent");
      me->clear();

      if(ctrl_pressed)
        me->add("Ctrl");
      if(shift_pressed)
        me->add("Shift");
      if(alt_pressed)
        me->add("Alt");

      me->add("Key")->setVal(kev->key());
      me->add("Text")->setVal(kev->text());
    }
  }

  emit signalKeyPress(this, kev->key());

  switch(kev->key())
  {
    case Qt::Key_Return:
    {
      //doReturn();
      //kev->ignore();
      break;
    }
  }

  completer->keyPressEvent(kev);

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

void ScriptEditor::checkForSave()
{
  if(timerId_)
  {
//    int n = QMessageBox::warning(0,
//                                 "Редактор",
//                                 "Значение изменено. Сохранить?",
//                                 QMessageBox::Yes | QMessageBox::No,
//                                 QMessageBox::Yes);

//    if(n == QMessageBox::Yes)
      save();
  }
}

void ScriptEditor::save()
{
  this->killTimer(timerId_);
  timerId_ = 0;

  if(sel_)
  {
    disconnectMem();
    sel_->setVal(this->toPlainText());
    connectMem();
  }
}

void ScriptEditor::doReturn()
{
  //QString str = this->toPlainText();
  //QStringList strList = str.split('\n');
  //QTextCharFormat format;
  //format.setVerticalAlignment(QTextCharFormat::AlignBaseline);
  //this->textCursor().setBlockCharFormat(format);
  //this->textCursor().insertBlock();

  //int pos = this->textCursor().position();
  //textCursor().movePosition(QTextCursor::Left);
  //this->textCursor().insertText("\n ");
}

void ScriptEditor::memory_change(MEWrapper *me, EMemoryChange idMsg)
{
  switch(idMsg)
  {
    case EMemoryChange::mcNone:
    case EMemoryChange::mcEditName:
    case EMemoryChange::mcAdd:
    case EMemoryChange::mcAddFrom:
    case EMemoryChange::mcUpdate:
    case mcMove:
    {
      break;
    }

    case EMemoryChange::mcSelect:
      if(me != sel_)
      {
        checkForSave();
        sel_ = me;
        showVal();
      }
      break;

    case EMemoryChange::mcClear:
    case EMemoryChange::mcEditVal:
      if(me == sel_)
        showVal();
      break;

    case EMemoryChange::mcDel:
      if(me == sel_)
      {
        sel_ = 0;
        showVal();
      }
      break;

    default:
      break;
  }
}

void ScriptEditor::on_textChanged()
{
  if(this->document()->isModified())
  {
    if(timerId_)
      this->killTimer(timerId_);
    timerId_ = this->startTimer(1000);
  }
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

void ScriptEditor::insertCompletion(const QString &)
{

}

void ScriptEditor::performCompletion()
{
  completer->performCompletion();
}

Intellect *ScriptEditor::intellect() const
{
  return intellect_;
}

void ScriptEditor::setIntellect(Intellect *intellect)
{
  intellect_ = intellect;
}

void ScriptEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
  QPainter painter(lineNumberArea_);
  painter.fillRect(event->rect(), Qt::lightGray);
  //lineNumberAreaPaintEvent() вызывается из LineNumberArea всякий раз при получении события рисования. Начинаем рисование фона виджета.

  QTextBlock block = firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
  int bottom = top + (int) blockBoundingRect(block).height();
  //Теперь пройдёмся по всем видимым строкам и отрисуем для каждой строки номер в дополнительной области.
  //Заметьте, что при редактировании обычного текста каждая строка будет состоять из одного QTextBlock;
  //тем не менее, если включен перенос строк, то строка может занимать несколько строк в области просмотра редактора текста.

  //Получим верхнюю и нижнюю y-координаты первого текстового блока,
  //и подгоним эти значения к высоте текущего текстового блока в каждой итерации цикла.

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

void ScriptEditor::showVal()
{
  this->blockSignals(true);
  if(sel_)
  {
    this->setPlainText(sel_->val().toString());
  }
  else
  {
    this->setPlainText("");
  }

  updateLineNumberAreaWidth(0);
  //updateLineNumberArea(contentsRect(), 0);

  this->blockSignals(false);
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

void Completer::keyPressEvent(QKeyEvent *event)
{
  if (completedAndSelected && handledCompletedAndSelected(event))
    return;
  completedAndSelected = false;
  if (popup()->isVisible()) {
    switch (event->key()) {
      case Qt::Key_Up: // Проваливаемся
      case Qt::Key_Down: // Проваливаемся
      case Qt::Key_Enter: // Проваливаемся
      case Qt::Key_Return: // Проваливаемся
      case Qt::Key_Escape: event->ignore(); return;
      default: popup()->hide(); break;
    }
  }
}

bool Completer::handledCompletedAndSelected(QKeyEvent *event)
{
  completedAndSelected = false;
  QTextCursor cursor = editor->textCursor();
  switch (event->key()) {
    case Qt::Key_Enter: // Проваливаемся
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
