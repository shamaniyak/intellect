#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "mytreewidgetitem.h"

namespace Ui {
class frmMemoryEditor;
}

class MemoryWrapper;

class FormMemoryEditor : public QWidget
{
  Q_OBJECT

public:
  explicit FormMemoryEditor(QWidget *parent = 0);
  ~FormMemoryEditor();

  void Init();

  MemoryWrapper *mem() const;
  void setMem(MemoryWrapper *mem);

protected:
  void ShowMemory();

private slots:
  void on_treeWidget_clicked(const QModelIndex &index);

  void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

  void on_txtVal_textChanged();

private:
  Ui::frmMemoryEditor *ui;
  MemoryWrapper *mem_;
};

#endif // FORM_H
