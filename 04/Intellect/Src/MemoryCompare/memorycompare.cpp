#include "memorycompare.h"

#include <Src/Memory/memorywrapper.h>

MemoryCompare::MemoryCompare(QObject *parent) : QSortFilterProxyModel(parent)
{

}

MemoryWrapper *MemoryCompare::srcMem() const
{
  return srcMem_;
}

void MemoryCompare::setSrcMem(MemoryWrapper *srcMem)
{
  srcMem_ = srcMem;
}

MemoryCompare::FilterType MemoryCompare::filter() const
{
  return filter_;
}

void MemoryCompare::setFilter(const FilterType &filter)
{
  filter_ = filter;
}

bool MemoryCompare::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
  if(NoFilter == filter_) return true;

  QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

  auto me = static_cast<MEWrapper*>(index.internalPointer());
  if(me) {
    QString path = me->getPath();
    QVariant val = me->val();

    if(srcMem_) {
      auto me1 = srcMem_->get(path);
    }
  }
}
