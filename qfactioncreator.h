#ifndef QFACTIONCREATOR_H
#define QFACTIONCREATOR_H

#include <QObject>
#include <QQmlParserStatus>
#include <QFAppDispatcher>
#include <QPointer>
#include "priv/qfsignalproxy.h"

class QFActionCreator : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

public:
    explicit QFActionCreator(QObject *parent = 0);

    QFAppDispatcher *dispatcher() const;
    void setDispatcher(QFAppDispatcher *value);

public slots:
    QString genKeyTable();

protected:
    void classBegin();
    void componentComplete();

private:
    QPointer<QFAppDispatcher> m_dispatcher;
    QList<QFSignalProxy*> m_proxyList;
};

#endif // QFACTIONCREATOR_H
