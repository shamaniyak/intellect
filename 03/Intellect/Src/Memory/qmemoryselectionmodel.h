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
  void memory_change(MEWrapper *me, EMemoryChange idMsg);
  void on_itemSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

private:
  MemoryWrapper *mem_ = nullptr;

  void init();
  void setSelected(MEWrapper *me);
  QModelIndex getIndexByMe(MEWrapper *me);
};

#endif // QMEMORYSELECTIONMODEL_H
