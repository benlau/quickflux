#ifndef QFAPPSCRIPTDISPATCHERWRAPPER_H
#define QFAPPSCRIPTDISPATCHERWRAPPER_H

#include <QQuickItem>
#include <QPointer>
#include "qfappdispatcher.h"

class QFAppScriptDispatcherWrapper : public QQuickItem
{
    Q_OBJECT
public:
    QFAppScriptDispatcherWrapper();

    QString type() const;
    void setType(const QString &type);

    QFAppDispatcher *dispatcher() const;
    void setDispatcher(QFAppDispatcher *dispatcher);

public slots:
    void dispatch(QJSValue arguments);

private:
    QString m_type;
    QPointer<QFAppDispatcher> m_dispatcher;

};

#endif // QFAPPSCRIPTDISPATCHERWRAPPER_H
