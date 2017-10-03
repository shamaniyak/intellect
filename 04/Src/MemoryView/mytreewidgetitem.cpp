#include "mytreewidgetitem.h"
#include "Src/Memory/mewrapper.h"

MyTreeWidgetItem::MyTreeWidgetItem()
{

}

MyTreeWidgetItem::MyTreeWidgetItem(QTreeWidget *tw, MEWrapper *me):
  QTreeWidgetItem(tw), me_(me)
{
  if(me)
    setText(0, me->name());
  fill();
}

MyTreeWidgetItem::MyTreeWidgetItem(MyTreeWidgetItem *parent, MEWrapper *me):
  QTreeWidgetItem(parent), me_(me)
{
  if(me)
    setText(0, me->name());
  fill();
}

MEWrapper *MyTreeWidgetItem::me() const
{
  return me_;
}

void MyTreeWidgetItem::fill()
{
  clear();

  if(!me_)
    return;

  int i =0;
  MEWrapper *me1 = me_->getByI(i++);
  while(me1)
  {
    new MyTreeWidgetItem(this, me1);
    me1 = me_->getByI(i++);
  }
}

void MyTreeWidgetItem::clear()
{
  auto children = takeChildren();
  for(auto item: children)
    removeChild(item);
}

void MyTreeWidgetItem::release()
{
  delete this;
}
