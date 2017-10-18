#ifndef LOGVIEW_H
#define LOGVIEW_H

#include <QObject>
#include <QPlainTextEdit>

class TAlgorithm;

class LogView : public QPlainTextEdit
{
  Q_OBJECT
public:
  explicit LogView(QWidget *parent = 0);
  ~LogView();

signals:

public slots:
  void addResult(const QString &str);

protected slots:

private:

};

#endif // LOGVIEW_H
