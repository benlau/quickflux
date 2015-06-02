#include <QtCore>
#include <QtQml>
#include <QVariant>
#include <QJSValue>
#include <QPointer>
#include "qfactiondispatcher.h"

QFActionDispatcher::QFActionDispatcher(QObject *parent) : QObject(parent)
{

}

QFActionDispatcher::~QFActionDispatcher()
{

}

void QFActionDispatcher::dispatch(QString name, QJSValue message)
{
    /*
    QMetaObject::invokeMethod(this,"received",Qt::QueuedConnection,
                              Q_ARG(QString,name),
                              Q_ARG(QJSValue,value));
                              */
    emit received(name,message);
}

static QObject *provider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    QFActionDispatcher* object = new QFActionDispatcher();

    return object;
}

class QFActionDispatcherRegisterHelper {

public:
    QFActionDispatcherRegisterHelper() {
        qmlRegisterSingletonType<QFActionDispatcher>("QuickFlux", 1, 0, "ActionDispatcher", provider);
    }
};

static QFActionDispatcherRegisterHelper registerHelper;
