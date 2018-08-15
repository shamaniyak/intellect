#ifndef PROCESSENVIRONMENT_H
#define PROCESSENVIRONMENT_H

#include <QObject>
#include <memory>

class QProcessEnvironment;

class ProcessEnvironment : public QObject
{
    Q_OBJECT
public:
    explicit ProcessEnvironment(QObject *parent = nullptr);

signals:

public slots:
    QStringList keys() const;

private:
    std::shared_ptr<QProcessEnvironment> m_processEnvironment;
};

#endif // PROCESSENVIRONMENT_H
