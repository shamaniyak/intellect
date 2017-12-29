#ifndef MEMORYCOMPARE_H
#define MEMORYCOMPARE_H

#include <QObject>
#include <QSortFilterProxyModel>

class MemoryWrapper;

class MemoryCompare : public QSortFilterProxyModel
{
  Q_OBJECT
public:

  // ��� ���������� ���������
  enum FilterType
  {
    NoFilter,           // ��� ����������
    Exist,              // ���������� � ����� �������
    NotExist,           // �� ���������� (������)
    NotEqualValue       // � ����� � ��� �� �������� ������ ����������
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
  // ��������� �� ������-��������
  MemoryWrapper *srcMem_ = nullptr;
  // ��� ����������
  FilterType filter_ = NoFilter;

  // QSortFilterProxyModel interface
protected:
  bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

#endif // MEMORYCOMPARE_H
