#ifndef QFACTIONDISPATCHER_H
#define QFACTIONDISPATCHER_H

#include <QObject>
#include <QVariantMap>
#include <QJSValue>

class QFActionDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit QFActionDispatcher(QObject *parent = 0);
    ~QFActionDispatcher();

    static QFActionDispatcher* instance();

signals:
    void received(QString name,QJSValue message);

public slots:
    Q_INVOKABLE void dispatch(QString name,QJSValue message);

private:
};

#endif // APPDISPATCHER_H
