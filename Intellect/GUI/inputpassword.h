#ifndef INPUTPASSWORD_H
#define INPUTPASSWORD_H

#include <QDialog>

class QLineEdit;

class InputPassword : public QDialog
{
  Q_OBJECT
public:
  explicit InputPassword(QWidget *parent = nullptr);

  bool isTrue(const QString &str);

signals:

public slots:
  int exec();

private:
  QString pass_;
  QLineEdit *edt_ = nullptr;

  // QWidget interface
protected:
  void keyPressEvent(QKeyEvent *event) override;
};

#endif // INPUTPASSWORD_H
