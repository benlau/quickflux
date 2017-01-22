#include <QtQml>
#include <QQmlEngine>
#include <QFAppDispatcher>
#include "qfactioncreator.h"
#include "qfstore.h"

QFStore::QFStore(QObject *parent) : QObject(parent)
{

}

QQmlListProperty<QObject> QFStore::children()
{
    return QQmlListProperty<QObject>(qobject_cast<QObject*>(this), m_children);
}

void QFStore::dispatch(QString type, QJSValue message)
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

    foreach(QObject* child , m_children) {
        QFStore* store = qobject_cast<QFStore*>(child);
        if (!store || store->bindSource() != this) {
            continue;
        }
        store->dispatch(type, message);
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

void QFStore::classBegin()
{

}

void QFStore::componentComplete()
{
    if (!m_bindSource.isNull()) {
        return;
    }

    QQmlEngine *engine = qmlEngine(this);
    Q_ASSERT(engine);

    QFStore* storeParent = qobject_cast<QFStore*>(parent());
    if (storeParent) {
        setBindSource(storeParent);
    } else {
        QFAppDispatcher* dispatcher = QFAppDispatcher::instance(engine);
        setBindSource(dispatcher);
    }
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
