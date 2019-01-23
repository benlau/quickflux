#include <QQmlExpression>
#include <QQmlEngine>
#include <QQmlContext>
#include "qfmiddlewarelist.h"
#include "priv/quickfluxfunctions.h"
#include "priv/qfmiddlewareshook.h"

/*!
   \qmltype MiddlewareList
   \inqmlmodule QuickFlux

\code
import QuickFlux 1.1
\endcode

MiddlewareList groups a list of Middleware and install to target Dispatcher / ActionCreator

Example Code

\code

import QtQuick 2.0
import QuickFlux 1.1
import "./actions"
import "./middlewares"

// main.qml

Item {
  MiddlewareList {
    applyTarget: AppDispatcher

    Middleware {
      id: debouncerMiddleware
   }

    Middleware {
      id: logger
    }
  }
}

\endcode

It is added since QuickFlux 1.1

    */

/*! \qmlproperty var MiddlewareList::applyTarget
 *
 * This property specific the target object that of middlewares should be applied.
 * It can be either of an ActionCreator or Dispatcher object.
 *
 */

QFMiddlewareList::QFMiddlewareList(QQuickItem* parent) : QQuickItem(parent)
{
    m_engine = nullptr;
}

void QFMiddlewareList::apply(QObject *source)
{
    setApplyTarget(source);
}

void QFMiddlewareList::next(int senderIndex, QString type, QJSValue message)
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

void QFMiddlewareList::classBegin()
{

}

void QFMiddlewareList::componentComplete()
{
    m_engine = qmlEngine(this);

    if (!m_applyTarget.isNull()) {
        setup();
    }
}

void QFMiddlewareList::setup()
{
    QFActionCreator *creator = nullptr;
    QFDispatcher* dispatcher = nullptr;

    creator = qobject_cast<QFActionCreator*>(m_applyTarget.data());

    if (creator) {
        dispatcher = creator->dispatcher();
    } else {
        dispatcher = qobject_cast<QFDispatcher*>(m_applyTarget.data());
    }

    if (creator == nullptr && dispatcher == nullptr) {
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
        m_dispatcher->setHook(nullptr);
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

QObject *QFMiddlewareList::applyTarget() const
{
    return m_applyTarget;
}

void QFMiddlewareList::setApplyTarget(QObject *applyTarget)
{
    m_applyTarget = applyTarget;
    if (!m_engine.isNull()) {
        setup();
    }

    emit applyTargetChanged();
}

