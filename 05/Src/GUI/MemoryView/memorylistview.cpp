#include "memorylistview.h"
#include "Src/Memory/mewrapper.h"

MemoryListView::MemoryListView(QWidget *parent) : QListWidget(parent)
{

}

MemoryWrapper *MemoryListView::mem() const
{
  return mem_;
}

void MemoryListView::setMem(MemoryWrapper *mem)
{
  //if(mem_ !=mem)
  {
    if(mem_)
      disconnect(mem_, &MemoryWrapper::on_change, this, &MemoryListView::on_memory_change);

    mem_ = mem;

    //FillTree(this, mem);

    if(mem_)
    {
      //setSelected(mem_->selected());

      connect(mem_, &MemoryWrapper::on_change, this, &MemoryListView::on_memory_change);
    }
  }
}

void MemoryListView::on_memory_change(const MEWrapper &me, EMemoryChange idMsg)
{
  if(me)
  {
    if(idMsg != mcNone)
    {

    }
  }
}
