#include "formMemoryEditor.h"
#include "ui_formMemoryEditor.h"
#include "memorytreeview.h"
#include <memorywrapper.h>
#include <mewrapper.h>
#include <QMenuBar>

FormMemoryEditor::FormMemoryEditor(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::frmMemoryEditor)
{
  ui->setupUi(this);
}

FormMemoryEditor::~FormMemoryEditor()
{
  delete ui;
}

void FormMemoryEditor::Init()
{
  QMenuBar *mnu = new QMenuBar(this);
  QMenu *pmenu = new QMenu("Menu");
  mnu->addMenu(pmenu);
  mnu->addMenu("Edit");
  mnu->show();

  QVBoxLayout *vl = new QVBoxLayout(this);
  vl->setMenuBar(mnu);
  //vl->addWidget(mnu);
  vl->addWidget(ui->splitter_2);

  setLayout(vl);
}

MemoryWrapper *FormMemoryEditor::mem() const
{
  return mem_;
}

void FormMemoryEditor::setMem(MemoryWrapper *mem)
{
  mem_ = mem;

  ShowMemory();
}

void FormMemoryEditor::ShowMemory()
{
  MemoryTreeView::FillTree(ui->treeWidget, mem_);
}

void FormMemoryEditor::on_treeWidget_clicked(const QModelIndex &index)
{

}

void FormMemoryEditor::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
  MyTreeWidgetItem *itm = dynamic_cast<MyTreeWidgetItem*>(item);
  MEWrapper *me = itm->me();

  ui->edtName->setText(me->name());
  ui->edtPath->setText(me->getPath());
  ui->txtVal->setPlainText(me->get_val().toString());
}

//void FormMemoryEditor::mousePressEvent ( QMouseEvent *event )
//{
//    if( event->button() == Qt::RightButton)
//    {
//        QMenu menu(this);
//        menu.addAction("ShowMessage");
//        menu.addAction("About Qt");
//        menu.addSeparator();
//        menu.addAction("Exit");
//        menu.exec(event->globalPos());
//    }
//    else
//        QLineEdit::mousePressEvent(event);
//}

void FormMemoryEditor::on_txtVal_textChanged()
{

}
