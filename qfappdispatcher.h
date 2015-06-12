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
    /// Listeners should listen on this signal to get the latest dispatched message from AppDispatcher
    void dispatched(QString name,QJSValue message);

public slots:
    /// Dispatch a message via Dispatcher
    /**
      @param name The name of the message
      @param message The message content
      @reentrant

      This function dispatch message by emitting the "dispatched" signal.
      Usually, direct connected slot function should be invoked immediately.
      But recursive call from slot functions will be processed differently.
      It will defer the signal emittion until the slot function is finished
      and go back to the top most dispatch() function in call tree. Such
      that the order of "dispatched" signal emission is in fast come, fast
      serve basis.
     */
    Q_INVOKABLE void dispatch(QString name,QJSValue message = QJSValue());

private:
    bool m_dispatching;
    QQueue<QPair<QString,QJSValue > > m_queue;

};

#endif // APPDISPATCHER_H
