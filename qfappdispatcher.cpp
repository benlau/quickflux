#include <QtCore>
#include <QtQml>
#include <QVariant>
#include <QJSValue>
#include <QPointer>
#include "qfappdispatcher.h"

QFAppDispatcher::QFAppDispatcher(QObject *parent) : QObject(parent)
{
    m_dispatching = false;
    nextListenerId = 1;
}

QFAppDispatcher::~QFAppDispatcher()
{

}

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
    if (!m_dispatching)
        return;

    int id = dispatchingListenerId;
    bool shouldWait = false;

    for (int i = 0 ; i < ids.size() ; i++) {
        int id = ids[i];
        if (pendingListeners.contains(id)) {
            shouldWait = true;
        } else if (waitingListeners.contains(id)) {
            qWarning() << "AppDispatcher: Cyclic dependency detected";
        }
    }

    if (!shouldWait) {
        return;
    }

    waitingListeners[id] = true;
    invokeListeners();
    waitingListeners.remove(id);
}

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
    while (iter.hasNext()) {
        iter.next();
        pendingListeners[iter.key()] = true;
    }

    invokeListeners();

    emit dispatched(type,message);
}

void QFAppDispatcher::invokeListeners()
{
    while (!pendingListeners.empty()) {
        int next = pendingListeners.firstKey();
        pendingListeners.remove(next);
        dispatchingListenerId = next;

        QFListener* listener = m_listeners[next].data();

        if (listener) {
            listener->dispatch(dispatchingMessageType,dispatchingMessage);
        }
    }
}

static QObject *provider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    QFAppDispatcher* object = new QFAppDispatcher();

    return object;
}

class QFAppDispatcherRegisterHelper {

public:
    QFAppDispatcherRegisterHelper() {
        qmlRegisterSingletonType<QFAppDispatcher>("QuickFlux", 1, 0, "AppDispatcher", provider);
    }
};

static QFAppDispatcherRegisterHelper registerHelper;
