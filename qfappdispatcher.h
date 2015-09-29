#ifndef QFAPPDISPATCHER_H
#define QFAPPDISPATCHER_H

#include <QObject>
#include <QVariantMap>
#include <QJSValue>
#include <QQueue>
#include <QPair>
#include <QQmlEngine>

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

      Dispatch a message with name via the AppDispatcher. Listeners should listen on the "dispatched"
      signal to be notified.

      Usually, it will emit "dispatched" signal immediately after calling dispatch(). However, if
      AppDispatcher is still dispatching messages, the new messages will be placed on a queue, and
      wait until it is finished. It guarantees the order of messages are arrived in sequence to
      listeners
     */
    Q_INVOKABLE void dispatch(QString name,QJSValue message = QJSValue());

    /// Obtain the singleton instance of AppDispatcher for specific QQmlEngine
    static QFAppDispatcher* instance(QQmlEngine* engine);

    /// Obtain a singleton object from package for specific QQmlEngine
    static QObject* singletonObject(QQmlEngine* engine,QString package,
                                    int versionMajor,
                                    int versionMinor,
                                    QString typeName);

private:
    bool m_dispatching;
    QQueue<QPair<QString,QJSValue > > m_queue;

};

#endif // APPDISPATCHER_H
