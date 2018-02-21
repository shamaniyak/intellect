#ifndef MEMORYTREEVIEW_H
#define MEMORYTREEVIEW_H

#include <QTreeWidget>
#include "Src/Memory/memorywrapper.h"

class MyTreeWidgetItem;
class MEWrapper;
class QMemoryModel;
class QMemorySelectionModel;
class MemoryCompareProxyModel;

class MemoryTreeView : public QTreeView
{
  Q_OBJECT
  Q_PROPERTY(MemoryWrapper* mem READ getMem WRITE setMem NOTIFY memChanged)
  Q_PROPERTY(MemoryWrapper* header READ memHeader)

public:
  explicit MemoryTreeView(QWidget *parent = 0);
  ~MemoryTreeView();

  QMenu *pmenu() const;

  MemoryWrapper *getMem() const;
  void setMem(MemoryWrapper *mem);

  void expandItems();
  void loadExpandItems(MEWrapper &me);
  void saveExpandItems(MEWrapper &me);

  MemoryWrapper *memHeader() const;

protected:
  void contextMenuEvent(QContextMenuEvent *pe) override;
  void mousePressEvent ( QMouseEvent *event ) override;

  void createModels();

  void createContextMenu();

  void connectSlots();

  void disconnectSlots();

  void initDragDrop();

  void setup();

signals:
  void contextMenuPopup();
  void memChanged();

public slots:
  void compareWith(MemoryWrapper *srcMem);
  void deleteCompare();

protected slots:

  void on_itemCollapsed(const QModelIndex &item);

  void on_itemExpanded(const QModelIndex &item);

  void on_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

private:
  QMenu *m_pmenu = 0;
  MemoryWrapper *mem_ = 0;
  QStringList expanded_;
  QMemoryModel *model_ = 0;
  QMemorySelectionModel *selectionModel_ = 0;
  MemoryCompareProxyModel *memoryCompare_ = 0;

  QWidget *createWidget(const QModelIndex &index);
};

#endif // MEMORYTREEVIEW_H
