#include <QtQml>
#include <QtCore>
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

        connect(m_target.data(),SIGNAL(received(QString,QJSValue)),
                this,SLOT(onDispatcherReceived(QString,QJSValue)));

    }


    emit targetChanged();
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
    return;
}

void QFAppListener::removeAllListener(QString name)
{
    if (name.isEmpty()) {
        mapping.clear();
    } else {
        mapping.remove(name);
    }
}

void QFAppListener::onDispatcherReceived(QString name, QJSValue message)
{
    if (!isEnabled())
        return;

    emit received(name,message);

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

class QFAppListenerRegisterHelper {

public:
    QFAppListenerRegisterHelper() {
        qmlRegisterType<QFAppListener>("QuickFlux", 1, 0, "QFAppListenerBase");
    }
};

static QFAppListenerRegisterHelper registerHelper;

