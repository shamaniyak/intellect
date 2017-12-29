#ifndef MEMORYCOMPARE_H
#define MEMORYCOMPARE_H

#include <QObject>
#include <QSortFilterProxyModel>

class MemoryWrapper;

class MemoryCompare : public QSortFilterProxyModel
{
  Q_OBJECT
public:

  // Тип фильтрации элементов
  enum FilterType
  {
    NoFilter,           // Без фильтрации
    Exist,              // Существуют в обоих моделях
    NotExist,           // Не существует (удален)
    NotEqualValue       // В одном и том же элементе данные отличаются
  };
  //Q_ENUM(FilterType)

  explicit MemoryCompare(QObject *parent = nullptr);

  MemoryWrapper *srcMem() const;
  void setSrcMem(MemoryWrapper *srcMem);

  FilterType filter() const;
  void setFilter(const FilterType &filter);

signals:

public slots:

private:
  // Указатель на память-источник
  MemoryWrapper *srcMem_ = nullptr;
  // Тип фильтрации
  FilterType filter_ = NoFilter;

  // QSortFilterProxyModel interface
protected:
  bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

#endif // MEMORYCOMPARE_H
