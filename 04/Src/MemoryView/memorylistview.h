#ifndef MEMORYLISTVIEW_H
#define MEMORYLISTVIEW_H

#include <QListWidget>
#include "Src/Memory/memorywrapper.h"

class MEWrapper;

class MemoryListView : public QListWidget
{
  Q_OBJECT
  Q_PROPERTY(MemoryWrapper* mem READ mem WRITE setMem NOTIFY memChanged)

public:
  explicit MemoryListView(QWidget *parent = 0);

  MemoryWrapper *mem() const;
  void setMem(MemoryWrapper *mem);

signals:
  void contextMenuPopup();
  void memChanged();

protected slots:
  void on_memory_change(MEWrapper *me, EMemoryChange idMsg);

private:
  MemoryWrapper *mem_ = 0;
};

#endif // MEMORYLISTVIEW_H
