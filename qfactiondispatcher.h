#ifndef QFACTIONDISPATCHER_H
#define QFACTIONDISPATCHER_H

#include <QObject>
#include <QVariantMap>
#include <QJSValue>
#include <QQueue>
#include <QPair>

class QFActionDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit QFActionDispatcher(QObject *parent = 0);
    ~QFActionDispatcher();

signals:
    void received(QString name,QJSValue message);

public slots:
    /// Dispatch a message to Dispatcher
    /**
      @param name The name of the message
      @param message The message content

      The message will be delivered to listeners via the "received" signal. This function
      guarantee the order of delivery even it is called recursively. It is safe to call
      within listener function.
     */
    Q_INVOKABLE void dispatch(QString name,QJSValue message);

private:
    bool m_dispatching;
    QQueue<QPair<QString,QJSValue > > m_queue;

};

#endif // APPDISPATCHER_H
