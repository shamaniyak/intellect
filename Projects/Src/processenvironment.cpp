#include "processenvironment.h"

#include <QDebug>
#include <QProcessEnvironment>

ProcessEnvironment::ProcessEnvironment(QObject *parent) : QObject(parent),
    m_processEnvironment(std::make_shared<QProcessEnvironment>())
{

}

QStringList ProcessEnvironment::keys() const
{
    return m_processEnvironment->systemEnvironment().keys();
}
