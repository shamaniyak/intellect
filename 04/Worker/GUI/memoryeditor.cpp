#include "memoryeditor.h"
#include <QGridLayout>
#include <QVBoxLayout>

MemoryEditor::MemoryEditor(QWidget *parent) : QWidget(parent)
{
  auto vl = new QVBoxLayout;
  gridLayout = new QGridLayout;
  gridLayout->setMargin(5);
  gridLayout->setSpacing(1);
  vl->addLayout(gridLayout);
  vl->addStretch(1);
  this->setLayout(vl);
}

void MemoryEditor::addWidget(QWidget *wgt)
{
  gridLayout->addWidget(wgt);
}

void MemoryEditor::addWidget(QWidget *wgt, int row, int column, int align)
{
  Qt::Alignment alignment = Qt::Alignment(align);
  gridLayout->addWidget(wgt, row, column, alignment);
}

void MemoryEditor::addWidget(QWidget *wgt, int row, int column, int rowSpan, int columnSpan, Qt::Alignment align)
{
  gridLayout->addWidget(wgt, row, column, rowSpan, columnSpan, align);
}
