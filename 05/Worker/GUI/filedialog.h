#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QObject>
#include <QFileDialog>

class FileDialog : public QFileDialog
{
  Q_OBJECT
  Q_PROPERTY(QStringList fileNames READ selectedFiles)
public:
  explicit FileDialog(QWidget *parent = 0);

signals:

public slots:
};

#endif // FILEDIALOG_H
