#include "inputpassword.h"

#include <QKeyEvent>
#include <QLineEdit>
#include <QVBoxLayout>

InputPassword::InputPassword(QWidget *parent) : QDialog(parent)
{

  setWindowTitle("Password");
  QVBoxLayout *layout = new QVBoxLayout();
  edt_ = new QLineEdit();
  edt_->setEchoMode(QLineEdit::Password);
  layout->addWidget(edt_);
  setLayout(layout);
}

bool InputPassword::isTrue(const QString &str)
{
  return(pass_ == str);
}

int InputPassword::exec()
{
  auto result = QDialog::exec();

  pass_ = edt_->text();

  return result;
}


void InputPassword::keyPressEvent(QKeyEvent *event)
{
  switch( event->key() )
  {
    case Qt::Key_Return:
      accept();
      break;
  }
}
