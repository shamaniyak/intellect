#ifndef MEMORYEDITOR_H
#define MEMORYEDITOR_H

#include <QObject>
#include <QWidget>

class QGridLayout;

class MemoryEditor : public QWidget
{
  Q_OBJECT
public:
  explicit MemoryEditor(QWidget *parent = 0);

signals:

public slots:
  void addWidget(QWidget *wgt);
  void addWidget(QWidget *wgt, int row, int column, int align = 0);
  void addWidget(QWidget *wgt, int row, int column, int rowSpan, int columnSpan, Qt::Alignment align = 0);

private:
  QGridLayout *gridLayout;
};

#endif // MEMORYEDITOR_H
