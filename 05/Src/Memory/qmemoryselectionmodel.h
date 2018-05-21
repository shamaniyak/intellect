#ifndef QMEMORYSELECTIONMODEL_H
#define QMEMORYSELECTIONMODEL_H

#include <QItemSelectionModel>

#include "memorywrapper.h"

class QAbstractItemModel;

class QMemorySelectionModel : public QItemSelectionModel
{
  Q_OBJECT

public:
  explicit QMemorySelectionModel(QAbstractItemModel *model = Q_NULLPTR);
  explicit QMemorySelectionModel(QAbstractItemModel *model, QObject *parent);
  virtual ~QMemorySelectionModel();

  MemoryWrapper *mem() const;
  void setMem(MemoryWrapper *mem);

private slots:
  void on_currentChanged(const QModelIndex &current, const QModelIndex &previous);
  void on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
  void onMemoryChanged(const ChangeEvent &ev);

private:
  MemoryWrapper *mem_ = nullptr;

  void init();
  void setSelected(const MEWrapper &me);
  QModelIndex getIndexByMe(const MEWrapper &me);
  MEWrapper getMeByIndex(const QModelIndex &index);
};

#endif // QMEMORYSELECTIONMODEL_H
