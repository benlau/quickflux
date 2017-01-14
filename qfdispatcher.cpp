#include <QtCore>
#include <QtQml>
#include <QVariant>
#include <QJSValue>
#include <QPointer>
#include "qfdispatcher.h"

/*!
   \qmltype AppDispatcher
   \inqmlmodule QuickFlux
   \brief Message Dispatcher

    AppDispatcher is a singleton object in QML scope for delivering action message.

 */

/*!
  \qmlsignal AppDispatcher::dispatched(string type, object message)

  This signal is emitted when a message is ready to dispatch by AppDispatcher.

  There has several methods to listen this signal:

Method 1 - Using Connections component

\code
import QuickFlux 1.0

Connections {
    target: AppDispatcher
    onDispatched: {
        switch (type) {
            case "OpenItem";
                // ...
                break;
            case "DeleteItem";
                // ...
                break;
        }
    }
}
\endcode

Method 2 - Using helper component, AppListener
\code
AppListener {
    filter: "ItemOpen";
    onDispatched: {
      /// ...
    }
}
\endcode

Method 3 - Using addListener
\code
Component.onCompleted: {
   AppDispatcher.addListener(function() {
     switch (type) {
        case "OpenItem";
          // ...
            break;
        case "DeleteItem";
          // ...
            break;
     }
   });
}
\endcode

 */

/*! \fn QFAppDispatcher::dispatched(QString type,QJSValue message)

  This signal is emitted when a message is ready to dispatch by AppDispatcher.

 */


/*!
  \class QFAppDispatcher
  \inmodule QuickFlux

  QFAppDispatcher is the C++ implementation of AppDispatcher in QML scope.
  You may need this class to setup communication between C++ and QML.

\code
#include "qfappdispatcher.h"
\endcode

 */

QFDispatcher::QFDispatcher(QObject *parent) : QObject(parent)
{
    m_dispatching = false;
    nextListenerId = 1;
}

QFDispatcher::~QFDispatcher()
{

}

/*!
  \qmlmethod AppDispatcher::dispatch(string type, object message)

  Dispatch a message with type via the AppDispatcher.
  The message will be placed on a queue and delivery via the "dispatched" signal.
  Listeners may listen on the "dispatched" signal directly,
  or using helper components like AppListener / AppScript to capture signal.

\code
MouseArea {
    anchors.fill: parent
    onClicked: {
        AppDispatcher.dispatch(ActionTypes.askToRemoveItem, { uid: uid });
    }
}
\endcode

Usually, it will emit "dispatched" signal immediately after calling dispatch().
However, if AppDispatcher is still dispatching messages,
the new messages will be placed on a queue,
and wait until it is finished.
It guarantees the order of messages are arrived in sequence to listeners

\code

AppListener {
    filter: ActionTypes.askToRemoveItem
    onDispatched: {
        if (options.skipRemoveConfirmation) {
            AppDispatcher.dispatch(ActionTypes.removeItem,message);
            // Because AppDispatcher is still dispatching ActionTypes.askToRemoveItem,
            // ActionTypes.removeItem will be placed in a queue and will dispatch when
            // all the listeners received current message.
        }
    }
}

\endcode

 */

void QFDispatcher::dispatch(QString type, QJSValue message)
{

#if (QT_VERSION == QT_VERSION_CHECK(5,7,1)) || (QT_VERSION == QT_VERSION_CHECK(5,8,0))

    /* A dirty hack to fix QTBUG-58133 and #13 issue.

       Reference:
       1. https://bugreports.qt.io/browse/QTBUG-58133
       2. https://github.com/benlau/quickflux/issues/13

     */
    if (message.isUndefined() && !m_engine.isNull()) {
        message = m_engine->toScriptValue<QVariant>(QVariant());
    }
#endif

    auto process = [=](QString type, QJSValue message) {
        if (m_hook.isNull()) {
            send(type, message);
        } else {
            m_hook->dispatch(type, message);
        }
    };

    if (m_dispatching) {
        m_queue.enqueue(QPair<QString,QJSValue> (type,message) );
        return;
    }

    m_dispatching = true;
    process(type,message);

    while (m_queue.size() > 0) {
        QPair<QString,QJSValue> pair = m_queue.dequeue();
        process(pair.first,pair.second);
    }
    m_dispatching = false;
}

/*!
  \qmlmethod AppDispatcher::waitFor(int listenerId)

  Waits for a callback specifed via the listenerId to be executed before continue execution of current callback.
  You should call this method only by a callback registered via addListener.

 */

void QFDispatcher::waitFor(QList<int> ids)
{
    if (!m_dispatching || ids.size() == 0)
        return;

    int id = dispatchingListenerId;

    waitingListeners[id] = true;
    invokeListeners(ids);
    waitingListeners.remove(id);
}

/*!

  \qmlmethod int AppDispatcher::addListener(func callback)

  Registers a callback to be invoked with every dispatched message. Returns a listener ID that can be used with waitFor().

 */

int QFDispatcher::addListener(QJSValue callback)
{
    QFListener* listener = new QFListener(this);
    listener->setCallback(callback);

    return addListener(listener);
}

/*! \fn int QFAppDispatcher::addListener(QFListener *listener)

  It is private API. Do not use it.

 */
int QFDispatcher::addListener(QFListener *listener)
{
    m_listeners[nextListenerId] = listener;
    listener->setListenerId(nextListenerId);
    return nextListenerId++;
}

/*!
  \qmlmethod AppDispatcher::removeListener(int listenerId)

  Remove a callback by the listenerId returned by addListener
 */

void QFDispatcher::removeListener(int id)
{
    if (m_listeners.contains(id)) {
        QFListener* listener = m_listeners[id].data();
        if (listener->parent() == this) {
            listener->deleteLater();
        }
        m_listeners.remove(id);
    }
}


/*! \fn QFAppDispatcher::dispatch(const QString& type, const QVariant& message)

  Dispatch a message with type via the AppDispatcher.
  The message will be placed on a queue and delivery via the "dispatched" signal.
  Listeners may listen on the "dispatched" signal directly,
  or using helper components like AppListener / AppScript to capture signal.
 */

void QFDispatcher::dispatch(const QString &type, const QVariant &message)
{
    if (m_engine.isNull()) {
        qWarning() << "QFAppDispatcher::dispatch() - Unexpected error: engine is not available.";
        return;
    }

    QJSValue value = m_engine->toScriptValue<QVariant>(message);

    dispatch(type, value);
}


void QFDispatcher::send(QString type, QJSValue message)
{
    dispatchingMessage = message;
    dispatchingMessageType = type;
    pendingListeners.clear();
    waitingListeners.clear();

    QMapIterator<int, QPointer<QFListener> > iter(m_listeners);
    QList<int> ids;
    while (iter.hasNext()) {
        iter.next();
        pendingListeners[iter.key()] = true;
        ids << iter.key();
    }

    invokeListeners(ids);

    emit dispatched(type,message);
}

void QFDispatcher::invokeListeners(QList<int> ids)
{
    for (int i = 0 ; i < ids.size() ; i++) {
        int next = ids.at(i);

        if (waitingListeners.contains(next)) {
            qWarning() << "AppDispatcher: Cyclic dependency detected";
        }

        if (!pendingListeners.contains(next))
            continue;

        pendingListeners.remove(next);
        dispatchingListenerId = next;

        QFListener* listener = m_listeners[next].data();

        if (listener) {
            listener->dispatch(this,dispatchingMessageType,dispatchingMessage);
        }
    }
}

QFHook *QFDispatcher::hook() const
{
    return m_hook;
}

void QFDispatcher::setHook(QFHook *hook)
{
    if (!m_hook.isNull()) {
        m_hook->disconnect(this);
    }

    m_hook = hook;

    if (!m_hook.isNull()) {
        connect(m_hook.data(), SIGNAL(send(QString,QJSValue)),
                this,SLOT(send(QString,QJSValue)));
    }

}

/*! \fn QQmlEngine *QFAppDispatcher::engine() const
  
  Obtain the associated engine to this dispatcher.
  
 */

QQmlEngine *QFDispatcher::engine() const
{
    return m_engine.data();
}

/*! \fn QFAppDispatcher::setEngine(QQmlEngine *engine)

  Set the associated \a engine. It is private API. Do not call it.

 */

void QFDispatcher::setEngine(QQmlEngine *engine)
{
    m_engine = engine;
}

