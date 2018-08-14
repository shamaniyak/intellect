#ifndef MYTREEWIDGETITEM_H
#define MYTREEWIDGETITEM_H

#include <QTreeWidgetItem>

#include <Interfaces/imemoryitem.h>

class MEWrapper;

class MyTreeWidgetItem : public QTreeWidgetItem, public IMemoryItem
{
public:
  MyTreeWidgetItem();
  MyTreeWidgetItem(QTreeWidget *tw, MEWrapper *me);
  MyTreeWidgetItem(MyTreeWidgetItem *parent, MEWrapper *me);

  MEWrapper *me() const;

  void fill();
  void clear();

private:
  MEWrapper *me_ = 0;

  // IMemoryItem interface
public:
  void release();
};

#endif // MYTREEWIDGETITEM_H
