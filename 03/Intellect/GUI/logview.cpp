#include "logview.h"
#include "talgorithm.h"

LogView::LogView(QWidget *parent) : QPlainTextEdit(parent)
{

}

TAlgorithm *LogView::getAlg() const
{
  return alg_;
}

void LogView::setAlg(TAlgorithm *value)
{
  if(alg_)
    alg_->disconnect(this);

  alg_ = value;

  if(alg_)
    connect(alg_, &TAlgorithm::addResult, this, &LogView::on_addResult);
}

void LogView::on_addResult(const QString &str)
{
  if(!str.isEmpty())
    this->appendPlainText(str);
}
