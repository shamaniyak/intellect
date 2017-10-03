#ifndef MEMORYEDITORDELEGATE_H
#define MEMORYEDITORDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class MemoryEditorDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  explicit MemoryEditorDelegate(QObject *parent = 0);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const override;

  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;

  void updateEditorGeometry(QWidget *editor,
      const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:

public slots:
};

#endif // MEMORYEDITORDELEGATE_H
