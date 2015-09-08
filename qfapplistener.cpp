#include <QtQml>
#include <QtCore>
#include "qfappdispatcher.h"
#include "qfapplistener.h"

QFAppListener::QFAppListener(QQuickItem *parent) : QQuickItem(parent)
{
}

QFAppListener::~QFAppListener()
{

}
QObject *QFAppListener::target() const
{
    return m_target;
}

void QFAppListener::setTarget(QObject *target)
{
    if (!m_target.isNull()) {
        m_target->disconnect(this);
    }

    m_target = target;

    if (!m_target.isNull()) {

        connect(m_target.data(),SIGNAL(dispatched(QString,QJSValue)),
                this,SLOT(onMessageReceived(QString,QJSValue)));

    }
}

QFAppListener *QFAppListener::on(QString name, QJSValue callback)
{
    QList<QJSValue> list;

    if (mapping.contains(name)) {
        list = mapping[name];
    }

    list.append(callback);

    mapping[name] = list;

    return this;
}

void QFAppListener::removeListener(QString name, QJSValue callback)
{
    if (!mapping.contains(name)) {
        return;
    };

    QList<QJSValue> list;
    list = mapping[name];

    int index = -1;
    for (int i = 0 ; i < list.size() ;i++) {
        if (list.at(i).equals(callback)) {
            index = i;
            break;
        }
    }

    if (index >=0 ) {
        list.removeAt(index);
        mapping[name] = list;
    }
}

void QFAppListener::removeAllListener(QString name)
{
    if (name.isEmpty()) {
        mapping.clear();
    } else {
        mapping.remove(name);
    }
}

void QFAppListener::componentComplete()
{
    QQuickItem::componentComplete();

    QQmlEngine *engine = qmlEngine(this);
    Q_ASSERT(engine);    

    QObject* dispatcher = qobject_cast<QObject*>(QFAppDispatcher::instance(engine));
    if (!dispatcher) {
        qWarning() << "Unknown error: Unable to access AppDispatcher";
    } else {
        setTarget(dispatcher);
    }
}

void QFAppListener::onMessageReceived(QString name, QJSValue message)
{
    if (!isEnabled())
        return;

    bool dispatch = true;

    QStringList rules = m_filters;
    if (!m_filter.isEmpty()) {
        rules.append(m_filter);
    }

    if (rules.size() > 0) {
        dispatch = false;

        for (int i = 0 ; i < rules.size() ; i++) {
            if (name == rules.at(i)) {
                dispatch = true;
                break;
            }
        }
    }

    if (dispatch) {
        emit dispatched(name,message);
    }

    // Listener registered with on() should not be affected by filter.

    if (!mapping.contains(name))
        return;

    QList<QJSValue> list = mapping[name];

    QList<QJSValue> arguments;
    arguments << message;

    Q_FOREACH(QJSValue value,list)  {
        if (value.isCallable()) {
            value.call(arguments);
        }
    }

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

