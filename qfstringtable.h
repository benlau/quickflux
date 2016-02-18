#ifndef QFSTRINGTABLE_H
#define QFSTRINGTABLE_H

#include <QObject>
#include <QQmlParserStatus>

class QFStringTable : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

public:
    explicit QFStringTable(QObject *parent = 0);

protected:
    void classBegin();
    void componentComplete();

};

#endif // QFSTRINGTABLE_H
