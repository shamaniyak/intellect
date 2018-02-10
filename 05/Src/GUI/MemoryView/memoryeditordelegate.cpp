#include "memoryeditordelegate.h"
#include "src/Memory/qmemorymodel.h"
#include "Src/GUI/ScriptEditor/scripteditor.h"

#include <QPainter>

MemoryEditorDelegate::MemoryEditorDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void MemoryEditorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  const QMemoryModel *model = qobject_cast<const QMemoryModel*>(index.model());
  Q_ASSERT(model);
  //const QDateTime &lastModified = model->lastModified(index);
  painter->save();
  painter->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
  if (option.state & QStyle::State_Selected) {
    painter->fillRect(option.rect, option.palette.highlight());
  }
  //const qreal diameter = qMin(option.rect.width(), option.rect.height());
  //const QRectF rect = clockRect(option.rect, diameter);
  //drawClockFace(painter, rect, lastModified);
  //drawClockHand(painter, rect.center(), diameter / 3.5, ((lastModified.time().hour() + (lastModified.time().minute() / 60.0))) * 30);
  //drawClockHand(painter, rect.center(), diameter / 2.5, lastModified.time().minute() * 6);
  //drawDate(painter, option, diameter, lastModified);
  painter->restore();

  QStyledItemDelegate::paint(painter, option, index);
}

QWidget *MemoryEditorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  if(index.column() == 0)
    return QStyledItemDelegate::createEditor(parent, option, index);

  ScriptEditor *editor = new ScriptEditor(parent);
  return editor;

  //return QStyledItemDelegate::createEditor(parent, option, index);
}

void MemoryEditorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  if(index.column() == 0) {
    QStyledItemDelegate::setEditorData(editor, index);
    return;
  }

  QString html = index.model()->data(index, Qt::EditRole).toString();
  ScriptEditor *lineEdit = qobject_cast<ScriptEditor*>(editor);
  Q_ASSERT(lineEdit);
  lineEdit->setPlainText(html);

  //QStyledItemDelegate::setEditorData(editor, index);
}

void MemoryEditorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  QStyledItemDelegate::setModelData(editor, model, index);
}

void MemoryEditorDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}


