#ifndef MEMORYCOMPAREPROXYMODEL_H
#define MEMORYCOMPAREPROXYMODEL_H

#include <QObject>
#include <QAbstractProxyModel>

class MemoryWrapper;
class MEWrapper;
//class QMemoryModel;

class MemoryCompareProxyModel : public QAbstractProxyModel
{
  Q_OBJECT
  Q_PROPERTY(FilterType filter READ filter WRITE setFilter)
public:

  // Тип фильтрации элементов
  enum FilterType
  {
    NoFilter,           // Без фильтрации
    Exist,              // Существуют в обоих моделях
    NotExist,           // Не существует (удален)
    NotEqualValue       // В одном и том же элементе данные отличаются
  };
  Q_ENUM(FilterType)

  explicit MemoryCompareProxyModel(QObject *parent = nullptr);

  MemoryWrapper *srcMem() const;
  void setSrcMem(MemoryWrapper *srcMem);

  FilterType filter() const;
  void setFilter(const FilterType &filter);

signals:

public slots:

private:
  // Указатель на текущую память, которую сравнивать
  MemoryWrapper *curMem_ = nullptr;
  // Указатель на память, с которой сравнивать
  MemoryWrapper *srcMem_ = nullptr;
  // Результирующая память
  MemoryWrapper *resultMem_ = nullptr;
  //QMemoryModel *resultMemoryModel_ = nullptr;
  // Тип фильтрации
  FilterType filter_ = NoFilter;

  // QAbstractProxyModel interface
public:
  QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
  QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

  // QAbstractItemModel interface
public:
  QModelIndex index(int row, int column, const QModelIndex &parent) const override;
  QModelIndex parent(const QModelIndex &child) const override;
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;

private:
  bool checkChangesRecurs(MEWrapper &me) const;
  MEWrapper getMeByIndex(const QModelIndex &index) const;
  void addFrom(MEWrapper &meFrom, MEWrapper &meTo);
};

#endif // MEMORYCOMPAREPROXYMODEL_H
