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

  TAlgorithm *getAlg() const;
  void setAlg(TAlgorithm *value);

signals:

public slots:

protected slots:
  void on_addResult(const QString &str);

private:
  TAlgorithm *alg_ = 0;
};

#endif // LOGVIEW_H
