#include "logview.h"
#include "Src/talgorithm.h"

#include <QtDebug>

LogView::LogView(QWidget *parent) : QPlainTextEdit(parent)
{

}

LogView::~LogView()
{
  qDebug() << "LogView::~LogView()";
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
