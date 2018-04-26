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
      mem_->disconnect(this);

    mem_ = mem;

    //FillTree(this, mem);

    if(mem_)
    {
      //setSelected(mem_->selected());

      connect(mem_, &MemoryWrapper::change, this, &MemoryListView::on_memory_change);
    }
  }
}

void MemoryListView::on_memory_change(const ChangeEvent &ev)
{
  if(ev.me)
  {
    if(ev.type != mcNone)
    {

    }
  }
}
