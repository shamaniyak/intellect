#include "memoryeditorwgt.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QtDebug>

MemoryEditorWgt::MemoryEditorWgt(QWidget *parent) : QWidget(parent)
{
  auto mainLayout = new QVBoxLayout;
  formLayout = new QFormLayout;
  formLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
  formLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
  formLayout->setFormAlignment(Qt::AlignHCenter | Qt::AlignTop);
  formLayout->setLabelAlignment(Qt::AlignLeft);
  //formLayout->setMargin(5);
  //formLayout->setSpacing(1);
  mainLayout->addLayout(formLayout);
  mainLayout->addStretch(1);
  this->setLayout(mainLayout);
}

QObject *MemoryEditorWgt::addLayout(int type, QObject *parent)
{
  auto p = qobject_cast<QBoxLayout*>(parent);
  auto l = new QBoxLayout(QBoxLayout::Direction(type));
  if(p)
    p->addLayout(l);
  return l;
}

void MemoryEditorWgt::addWidget(QWidget *wgt, QObject *layout)
{
  auto l = qobject_cast<QBoxLayout*>(layout);
  if(l)
    l->addWidget(wgt);
}

void MemoryEditorWgt::addWidget(QWidget *wgt, int row, int column, int align)
{
  Qt::Alignment alignment = Qt::Alignment(align);
  //gridLayout->addWidget(wgt, row, column, alignment);
}

void MemoryEditorWgt::addWidget(QWidget *wgt, int row, int column, int rowSpan, int columnSpan, Qt::Alignment align)
{
  //gridLayout->addWidget(wgt, row, column, rowSpan, columnSpan, align);
}

void MemoryEditorWgt::addSpacer(QObject *parent)
{
  //auto s = new QVerticalSpaser();
}

void MemoryEditorWgt::setLayout(QObject *layout)
{
  auto l = qobject_cast<QBoxLayout*>(layout);
  if(l)
    QWidget::setLayout(l);
}

void MemoryEditorWgt::addRow(const QString &name, QWidget *w)
{
  qDebug() << name;
  w->setParent(0);
  formLayout->addRow(name, w);
  formLayout->update();
}
