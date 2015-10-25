#include <QQmlExpression>
#include "priv/qfappscriptrunnable.h"

QFAppScriptRunnable::QFAppScriptRunnable(QObject *parent) : QObject(parent)
{
}

QJSValue QFAppScriptRunnable::script() const
{
    return m_script;
}

void QFAppScriptRunnable::setScript(const QJSValue &script)
{
    m_script = script;
}

QString QFAppScriptRunnable::type() const
{
    return m_type;
}

void QFAppScriptRunnable::setType(const QString &type)
{
    m_type = type;
}

void QFAppScriptRunnable::run(QJSValue message)
{
    QJSValueList args;
    args << message;
    m_script.call(args);
}

