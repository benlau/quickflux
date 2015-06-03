#ifndef QFAPPDISPATCHER_H
#define QFAPPDISPATCHER_H

#include <QObject>
#include <QVariantMap>
#include <QJSValue>
#include <QQueue>
#include <QPair>

/// Message Dispatcher

class QFAppDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit QFAppDispatcher(QObject *parent = 0);
    ~QFAppDispatcher();

signals:
    void received(QString name,QJSValue message);

public slots:
    /// Dispatch a message via Dispatcher
    /**
      @param name The name of the message
      @param message The message content
      @reentrant

      This function dispatch message via the "received" signal.
      Direct connected slot functions will be invoked by first come first
      served basis. It is safe to call dispatch() within slot function,
      the order will be preserved.
     */
    Q_INVOKABLE void dispatch(QString name,QJSValue message);

private:
    bool m_dispatching;
    QQueue<QPair<QString,QJSValue > > m_queue;

};

#endif // APPDISPATCHER_H
