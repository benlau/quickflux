#include <QQmlExpression>
#include <QQmlEngine>
#include <QQmlContext>
#include "qfmiddlewares.h"
#include "priv/quickfluxfunctions.h"
#include "priv/qfmiddlewareshook.h"

QFMiddlewares::QFMiddlewares()
{
    m_engine = 0;
}

void QFMiddlewares::apply(QObject *source)
{
    QFActionCreator *creator = 0;
    QFDispatcher* dispatcher = 0;

    creator = qobject_cast<QFActionCreator*>(source);
    if (creator) {
        dispatcher = creator->dispatcher();
    } else {
        dispatcher = qobject_cast<QFDispatcher*>(source);
    }

    if (creator == 0 && dispatcher == 0) {
        qWarning() << "Middlewares:bind(): Invalid input";
    }

    setup(creator, dispatcher);
}

void QFMiddlewares::next(int senderIndex, QString type, QJSValue message)
{
    QJSValueList args;

    args << QJSValue(senderIndex + 1);
    args << QJSValue(type);
    args << message;
    QJSValue result = invoke.call(args);
    if (result.isError()) {
        QuickFlux::printException(result);
    }
}

void QFMiddlewares::classBegin()
{

}

void QFMiddlewares::componentComplete()
{
    m_engine = qmlEngine(this);
}

void QFMiddlewares::setup(QFActionCreator *creator, QFDispatcher *dispatcher)
{

    if (m_actionCreator.data() == creator &&
        m_dispatcher.data() == dispatcher) {
        // Nothing changed.
        return;
    }

    if (!m_actionCreator.isNull() &&
        m_actionCreator.data() != creator) {
        // in case the action creator is not changed, do nothing.
        m_actionCreator->disconnect(this);
    }

    if (!m_dispatcher.isNull() &&
        m_dispatcher.data() != dispatcher) {
        QFHook* hook = m_dispatcher->hook();
        m_dispatcher->setHook(0);
        m_dispatcher->disconnect(this);
        if (hook) {
            delete hook;
        }
    }

    //@FIXME Listen for action::dispatcherChanged signal
    m_actionCreator = creator;
    m_dispatcher = dispatcher;

    if (!m_dispatcher.isNull()) {

        QFMiddlewaresHook* hook = new QFMiddlewaresHook();
        hook->setParent(this);
        hook->setup(m_engine.data(), this);

        if (!m_dispatcher.isNull()) {
            m_dispatcher->setHook(hook);
        }
    }
}

