#ifndef MEMORYEDITOR_H
#define MEMORYEDITOR_H

#include <QObject>
#include <QWidget>

class QFormLayout;

class MemoryEditor : public QWidget
{
  Q_OBJECT
public:
  explicit MemoryEditor(QWidget *parent = 0);

signals:

public slots:
  QObject * addLayout(int type, QObject *parent = nullptr);
  void addWidget(QWidget *wgt, QObject *layout=0);
  void addWidget(QWidget *wgt, int row, int column, int align = 0);
  void addWidget(QWidget *wgt, int row, int column, int rowSpan, int columnSpan, Qt::Alignment align = 0);

  void addSpacer(QObject *parent);

  void setLayout(QObject *layout);

  void addRow(const QString &name, QWidget *w);

private:
  QFormLayout *formLayout;
};

#endif // MEMORYEDITOR_H
