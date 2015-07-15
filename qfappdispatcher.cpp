#include <QtCore>
#include <QtQml>
#include <QVariant>
#include <QJSValue>
#include <QPointer>
#include "qfappdispatcher.h"

QFAppDispatcher::QFAppDispatcher(QObject *parent) : QObject(parent)
{
    m_dispatching = false;
}

QFAppDispatcher::~QFAppDispatcher()
{

}

void QFAppDispatcher::dispatch(QString name, QJSValue message)
{
    if (m_dispatching) {
        m_queue.enqueue(QPair<QString,QJSValue> (name,message) );
        return;
    }

    m_dispatching = true;
    emit dispatched(name,message);

    while (m_queue.size() > 0) {
        QPair<QString,QJSValue> pair = m_queue.dequeue();
        emit dispatched(pair.first,pair.second);
    }
    m_dispatching = false;
}

QFAppDispatcher *QFAppDispatcher::instance(QQmlEngine *engine)
{
    QFAppDispatcher *dispatcher = 0;

    QQmlComponent comp (engine);
    comp.setData("import QtQuick 2.0\nimport QuickFlux 1.0;QtObject { property var dispatcher : AppDispatcher }" ,QUrl());

    QObject* holder = comp.create();
    if (!holder) {
        qWarning() << "Unknown error: Unable to access AppDispatcher: " << comp.errorString();
        return dispatcher;
    }

    dispatcher = holder->property("dispatcher").value<QFAppDispatcher*>();
    holder->deleteLater();

    if (!dispatcher) {
        qWarning() << "#QuickFlux: Unknown error: Unable to access AppDispatcher";
    }

    return dispatcher;
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
