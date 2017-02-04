#include <QQmlEngine>
#include "qfmiddleware.h"
#include "priv/quickfluxfunctions.h"

QFMiddleware::QFMiddleware(QQuickItem* parent) : QQuickItem(parent),  m_filterFunctionEnabled(false)
{

}

void QFMiddleware::next(QString type, QJSValue message)
{
    QQmlEngine* engine = qmlEngine(this);
    QF_PRECHECK_DISPATCH(engine, type, message);

    if (m_nextCallback.isCallable()) {
        QJSValueList args;
        args << type;
        args << message;
        m_nextCallback.call(args);
    }
}

QJSValue QFMiddleware::nextCallback() const
{
    return m_nextCallback;
}

void QFMiddleware::setNextCallback(const QJSValue &value)
{
    m_nextCallback = value;
    emit _nextCallbackChanged();
}
