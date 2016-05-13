#ifndef QFACTIONCREATOR_H
#define QFACTIONCREATOR_H

#include <QObject>
#include <QQmlParserStatus>

class QFActionCreator : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

public:
    explicit QFActionCreator(QObject *parent = 0);

public slots:
    QString genKeyTable();

protected:
    void classBegin();
    void componentComplete();

};

#endif // QFACTIONCREATOR_H
