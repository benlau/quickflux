#include <QtCore>
#include <QtQml>
#include <QVariant>
#include <QJSValue>
#include <QPointer>
#include "qfappdispatcher.h"

/*!
   \qmltype AppDispatcher
   \inqmlmodule QuickFlux
   \brief Message Dispatcher

    AppDispatcher is a singleton object in QML scope for action message delivery.

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
QFAppDispatcher::QFAppDispatcher(QObject *parent) : QObject(parent)
{
    m_dispatching = false;
    nextListenerId = 1;
}

QFAppDispatcher::~QFAppDispatcher()
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

void QFAppDispatcher::dispatch(QString type, QJSValue message)
{
    if (m_dispatching) {
        m_queue.enqueue(QPair<QString,QJSValue> (type,message) );
        return;
    }

    m_dispatching = true;
    emitDispatched(type,message);

    while (m_queue.size() > 0) {
        QPair<QString,QJSValue> pair = m_queue.dequeue();
        emitDispatched(pair.first,pair.second);
    }
    m_dispatching = false;
}

void QFAppDispatcher::waitFor(QList<int> ids)
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

int QFAppDispatcher::addListener(QJSValue callback)
{
    QFListener* listener = new QFListener(this);
    listener->setCallback(callback);

    return addListener(listener);
}

int QFAppDispatcher::addListener(QFListener *listener)
{
    m_listeners[nextListenerId] = listener;
    listener->setListenerId(nextListenerId);
    return nextListenerId++;
}

/*!
  \qmlmethod AppDispatcher::removeListener(int listenerId)

  Remove a callback by the listenerId returned by addListener
 */

void QFAppDispatcher::removeListener(int id)
{
    if (m_listeners.contains(id)) {
        QFListener* listener = m_listeners[id].data();
        if (listener->parent() == this) {
            listener->deleteLater();
        }
        m_listeners.remove(id);
    }
}

QFAppDispatcher *QFAppDispatcher::instance(QQmlEngine *engine)
{
    QFAppDispatcher *dispatcher = qobject_cast<QFAppDispatcher*>(singletonObject(engine,"QuickFlux",1,0,"AppDispatcher"));

    return dispatcher;
}

QObject *QFAppDispatcher::singletonObject(QQmlEngine *engine, QString package, int versionMajor, int versionMinor, QString typeName)
{
    QString pattern  = "import QtQuick 2.0\nimport %1 %2.%3;QtObject { property var object : %4 }";

    QString qml = pattern.arg(package).arg(versionMajor).arg(versionMinor).arg(typeName);

    QObject* holder = 0;

    QQmlComponent comp (engine);
    comp.setData(qml.toUtf8(),QUrl());
    holder = comp.create();

    if (!holder) {
        qWarning() << QString("QuickFlux: Failed to gain singleton object: %1").arg(typeName);
        qWarning() << QString("Error: ") << comp.errorString();
        return 0;
    }

    QObject*object = holder->property("object").value<QObject*>();
    holder->deleteLater();

    if (!object) {
        qWarning() << QString("QuickFlux: Failed to gain singleton object: %1").arg(typeName);
        qWarning() << QString("Error: Unknown");
    }

    return object;
}

void QFAppDispatcher::emitDispatched(QString type, QJSValue message)
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

void QFAppDispatcher::invokeListeners(QList<int> ids)
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

