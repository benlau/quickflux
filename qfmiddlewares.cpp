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
    setApplyTarget(source);
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

    if (!m_applyTarget.isNull()) {
        setup();
    }
}

void QFMiddlewares::setup()
{
    QFActionCreator *creator = 0;
    QFDispatcher* dispatcher = 0;

    creator = qobject_cast<QFActionCreator*>(m_applyTarget.data());
    if (creator) {
        dispatcher = creator->dispatcher();
    } else {
        dispatcher = qobject_cast<QFDispatcher*>(m_applyTarget.data());
    }

    if (creator == 0 && dispatcher == 0) {
        qWarning() << "Middlewares.apply(): Invalid input";
    }

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

    m_actionCreator = creator;
    m_dispatcher = dispatcher;

    if (!m_actionCreator.isNull()) {
        connect(m_actionCreator.data(),SIGNAL(dispatcherChanged()),
                this,SLOT(setup()));
    }

    if (!m_dispatcher.isNull()) {
        QFMiddlewaresHook* hook = new QFMiddlewaresHook();
        hook->setParent(this);
        hook->setup(m_engine.data(), this);

        if (!m_dispatcher.isNull()) {
            m_dispatcher->setHook(hook);
        }
    }
}

QObject *QFMiddlewares::applyTarget() const
{
    return m_applyTarget;
}

void QFMiddlewares::setApplyTarget(QObject *applyTarget)
{
    m_applyTarget = applyTarget;
    if (!m_engine.isNull()) {
        setup();
    }

    emit applyTargetChanged();
}

