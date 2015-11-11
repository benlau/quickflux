#include <QtQml>
#include <QtCore>
#include "qfappdispatcher.h"
#include "qfapplistener.h"

QFAppListener::QFAppListener(QQuickItem *parent) : QQuickItem(parent)
{
    m_alwaysOn = false;
    m_listener = 0;
    m_listenerId = 0;
}

QFAppListener::~QFAppListener()
{
    if (!m_target.isNull()) {
        m_target->removeListener(m_listenerId);
    }
}

QObject *QFAppListener::target() const
{
    return m_target;
}

void QFAppListener::setTarget(QFAppDispatcher *target)
{
    if (!m_target.isNull()) {
        m_target->removeListener(m_listenerId);
        m_listener->disconnect(this);
        m_listener->deleteLater();
        m_listener = 0;
        setListenerId(0);
    }

    m_target = target;

    if (!m_target.isNull()) {

        m_listener = new QFListener(this);

        setListenerId(m_target->addListener(m_listener));

        setListenerWaitFor();

        connect(m_listener,SIGNAL(dispatched(QString,QJSValue)),
                this,SLOT(onMessageReceived(QString,QJSValue)));
    }
}

QFAppListener *QFAppListener::on(QString type, QJSValue callback)
{
    QList<QJSValue> list;

    if (mapping.contains(type)) {
        list = mapping[type];
    }

    list.append(callback);

    mapping[type] = list;

    return this;
}

void QFAppListener::removeListener(QString type, QJSValue callback)
{
    if (!mapping.contains(type)) {
        return;
    };

    QList<QJSValue> list;
    list = mapping[type];

    int index = -1;
    for (int i = 0 ; i < list.size() ;i++) {
        if (list.at(i).equals(callback)) {
            index = i;
            break;
        }
    }

    if (index >=0 ) {
        list.removeAt(index);
        mapping[type] = list;
    }
}

void QFAppListener::removeAllListener(QString type)
{
    if (type.isEmpty()) {
        mapping.clear();
    } else {
        mapping.remove(type);
    }
}

void QFAppListener::componentComplete()
{
    QQuickItem::componentComplete();

    QQmlEngine *engine = qmlEngine(this);
    Q_ASSERT(engine);    

    QFAppDispatcher* dispatcher = qobject_cast<QFAppDispatcher*>(QFAppDispatcher::instance(engine));
    if (!dispatcher) {
        qWarning() << "Unknown error: Unable to access AppDispatcher";
    } else {
        setTarget(dispatcher);
    }
}

void QFAppListener::onMessageReceived(QString type, QJSValue message)
{
    if (!isEnabled() && !m_alwaysOn)
        return;

    bool dispatch = true;

    QStringList rules = m_filters;
    if (!m_filter.isEmpty()) {
        rules.append(m_filter);
    }

    if (rules.size() > 0) {
        dispatch = false;

        for (int i = 0 ; i < rules.size() ; i++) {
            if (type == rules.at(i)) {
                dispatch = true;
                break;
            }
        }
    }

    if (dispatch) {
        emit dispatched(type,message);
    }

    // Listener registered with on() should not be affected by filter.

    if (!mapping.contains(type))
        return;

    QList<QJSValue> list = mapping[type];

    QList<QJSValue> arguments;
    arguments << message;

    Q_FOREACH(QJSValue value,list)  {
        if (value.isCallable()) {
            value.call(arguments);
        }
    }

}

void QFAppListener::setListenerWaitFor()
{
    if (!m_listener) {
        return;
    }

    m_listener->setWaitFor(m_waitFor);
}


QList<int> QFAppListener::waitFor() const
{
    return m_waitFor;
}

void QFAppListener::setWaitFor(const QList<int> &waitFor)
{
    m_waitFor = waitFor;
    setListenerWaitFor();
    emit waitForChanged();
}

int QFAppListener::listenerId() const
{
    return m_listenerId;
}

void QFAppListener::setListenerId(int listenerId)
{
    m_listenerId = listenerId;
    emit listenerIdChanged();
}

bool QFAppListener::alwaysOn() const
{
    return m_alwaysOn;
}

void QFAppListener::setAlwaysOn(bool alwaysOn)
{
    m_alwaysOn = alwaysOn;
    emit alwaysOnChanged();
}

QStringList QFAppListener::filters() const
{
    return m_filters;
}

void QFAppListener::setFilters(const QStringList &filters)
{
    m_filters = filters;
    emit filtersChanged();
}

QString QFAppListener::filter() const
{
    return m_filter;
}

void QFAppListener::setFilter(const QString &filter)
{
    m_filter = filter;
    emit filterChanged();
}


class QFAppListenerRegisterHelper {
    
public:
    QFAppListenerRegisterHelper() {
        qmlRegisterType<QFAppListener>("QuickFlux", 1, 0, "AppListener");
    }
};

static QFAppListenerRegisterHelper registerHelper;

