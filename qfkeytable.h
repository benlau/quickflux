#pragma once

#include <QObject>
#include <QQmlParserStatus>

class QFKeyTable : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

public:
    explicit QFKeyTable(QObject *parent = 0);

protected:
    void classBegin();
    void componentComplete();

};
