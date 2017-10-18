#include "logview.h"
#include "Src/talgorithm.h"

#include <QtDebug>

LogView::LogView(QWidget *parent) : QPlainTextEdit(parent)
{

}

LogView::~LogView()
{
//  qDebug() << "LogView::~LogView()";
}

void LogView::addResult(const QString &str)
{
  if(!str.isEmpty())
    this->appendPlainText(str);
}
