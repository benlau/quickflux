#include <QtQml>
#include <QQmlEngine>
#include <QFAppDispatcher>
#include "priv/quickfluxfunctions.h"
#include "qfactioncreator.h"
#include "qfstore.h"

QFStore::QFStore(QObject *parent) : QObject(parent) , m_filterFunctionEnabled(false)
{

}

QQmlListProperty<QObject> QFStore::children()
{
    return QQmlListProperty<QObject>(qobject_cast<QObject*>(this), m_children);
}

void QFStore::dispatch(QString type, QJSValue message)
{
    QQmlEngine* engine = qmlEngine(this);
    QF_PRECHECK_DISPATCH(engine, type, message);

    foreach(QObject* child , m_children) {
        QFStore* store = qobject_cast<QFStore*>(child);
        if (!store) {
            continue;
        }
        store->dispatch(type, message);
    }

    foreach(QObject* child , m_redispatchTargets) {
        QFStore* store = qobject_cast<QFStore*>(child);

        if (!store) {
            continue;
        }
        store->dispatch(type, message);
    }

    if (m_filterFunctionEnabled) {
        const QMetaObject *meta = metaObject();
        QByteArray signature;
        int index;

        signature = QMetaObject::normalizedSignature(QString("%1(QVariant)").arg(type).toUtf8().constData());
        if ( (index = meta->indexOfMethod(signature.constData())) >= 0 ) {
            QMetaMethod method = meta->method(index);
            QVariant value = QVariant::fromValue<QJSValue>(message);

            method.invoke(this,Qt::DirectConnection, Q_ARG(QVariant, value));
        }

        signature = QMetaObject::normalizedSignature(QString("%1()").arg(type).toUtf8().constData());
        if ( (index = meta->indexOfMethod(signature.constData())) >= 0 ) {
            QMetaMethod method = meta->method(index);

            method.invoke(this);
        }
    }

    emit dispatched(type, message);
}

void QFStore::bind(QObject *source)
{
    setBindSource(source);
}

void QFStore::setup()
{
    QFActionCreator *creator = 0;
    QFDispatcher* dispatcher = 0;

    creator = qobject_cast<QFActionCreator*>(m_bindSource.data());

    if (creator) {
        dispatcher = creator->dispatcher();
    } else {
        dispatcher = qobject_cast<QFDispatcher*>(m_bindSource.data());
    }

    if (m_actionCreator.data() == creator &&
        m_dispatcher.data() == dispatcher) {
        // Nothing changed.
        return;
    }

    if (!m_actionCreator.isNull() &&
        m_actionCreator.data() != creator) {
        m_actionCreator->disconnect(this);
    }

    if (!m_dispatcher.isNull() &&
        m_dispatcher.data() != dispatcher) {
        m_dispatcher->disconnect(this);
    }

    m_actionCreator = creator;
    m_dispatcher = dispatcher;

    if (!m_actionCreator.isNull()) {
        connect(m_actionCreator.data(),SIGNAL(dispatcherChanged()),
                this,SLOT(setup()));
    }

    if (!m_dispatcher.isNull()) {
        connect(dispatcher,SIGNAL(dispatched(QString,QJSValue)),
                this,SLOT(dispatch(QString,QJSValue)));
    }

}

QQmlListProperty<QObject> QFStore::redispatchTargets()
{
    return QQmlListProperty<QObject>(qobject_cast<QObject*>(this), m_redispatchTargets);
}

QObject *QFStore::bindSource() const
{
    return m_bindSource.data();
}

void QFStore::setBindSource(QObject *source)
{
    m_bindSource = source;
    setup();
    emit bindSourceChanged();
}
