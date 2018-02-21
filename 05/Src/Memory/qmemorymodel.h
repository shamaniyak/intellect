#ifndef QMEMORYMODEL_H
#define QMEMORYMODEL_H

#include "memorywrapper.h"

#include <QObject>
#include <QAbstractItemModel>

class QMemoryModel : public QAbstractItemModel
{
  Q_OBJECT
  Q_PROPERTY(MemoryWrapper* memory READ mem WRITE setMem)
  Q_PROPERTY(MemoryWrapper* header READ getHeaderInfo)

  typedef QAbstractItemModel inherited;
public:
  explicit QMemoryModel(QObject *parent = 0);
  ~QMemoryModel();

signals:

public slots:

  // QAbstractItemModel interface
public:
  QModelIndex index(int row, int column, const QModelIndex &parent) const override;
  QModelIndex parent(const QModelIndex &child) const override;

  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;

  QVariant data(const QModelIndex &index, int role) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role) override;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

  bool canFetchMore(const QModelIndex &parent) const override;
  void fetchMore(const QModelIndex &parent) override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;

  bool hasChildren(const QModelIndex &parent) const override;

  Qt::DropActions  supportedDragActions() const override;
  Qt::DropActions supportedDropActions() const override;

  MemoryWrapper *mem() const;
  void setMem(MemoryWrapper *mem);

  MemoryWrapper *getHeaderInfo() const;

  QModelIndex getIndexByMe(const MEWrapper &me);
  MEWrapper getMeByIndex(const QModelIndex &index) const;

  QHash<int,QByteArray> roleNames() const override;

protected:

  void updateMe(const MEWrapper &me);

private slots:
  void memory_change(MEWrapper &me, EMemoryChange idMsg);
  void onMemoryChange(const ChangeEvent &event);
  void onHeaderChange(const MEWrapper &me, EMemoryChange idMsg);

private:
  enum Columns
  {
    NameColumn,
    ValueColumn,
    PathColumn,

    ColumnCount
  };

  MemoryWrapper *mem_ = nullptr;
  MemoryWrapper *headerInfo_ = nullptr;

  // QAbstractItemModel interface
public:
  QStringList mimeTypes() const override;
  QMimeData *mimeData(const QModelIndexList &indexes) const override;
  bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
  bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
  bool insertRows(int row, int count, const QModelIndex &parent) override;

};

#endif // QMEMORYMODEL_H
