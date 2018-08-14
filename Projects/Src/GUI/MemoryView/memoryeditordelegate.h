#ifndef MEMORYEDITORDELEGATE_H
#define MEMORYEDITORDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class MemoryEditorDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  explicit MemoryEditorDelegate(QObject *parent = 0);

  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const override;

  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;

  void updateEditorGeometry(QWidget *editor,
      const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:

public slots:

private:


};

#endif // MEMORYEDITORDELEGATE_H
