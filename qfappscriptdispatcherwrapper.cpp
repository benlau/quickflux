#include <QQmlEngine>
#include "qfappdispatcher.h"
#include "priv/qfappscriptdispatcherwrapper.h"

QFAppScriptDispatcherWrapper::QFAppScriptDispatcherWrapper()
{

}

QString QFAppScriptDispatcherWrapper::type() const
{
    return m_type;
}

void QFAppScriptDispatcherWrapper::setType(const QString &type)
{
    m_type = type;
}

void QFAppScriptDispatcherWrapper::dispatch(QJSValue arguments)
{
    if (m_dispatcher.isNull()) {
        qWarning() << "AppScript::Unexcepted condition: AppDispatcher is not present.";
        return;
    }

    m_dispatcher->dispatch(m_type,arguments);
}

QFAppDispatcher *QFAppScriptDispatcherWrapper::dispatcher() const
{
    return m_dispatcher;
}

void QFAppScriptDispatcherWrapper::setDispatcher(QFAppDispatcher *dispatcher)
{
    m_dispatcher = dispatcher;
}
