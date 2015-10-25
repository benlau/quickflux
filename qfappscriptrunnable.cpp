#include <QQmlExpression>
#include "priv/qfappscriptrunnable.h"

QFAppScriptRunnable::QFAppScriptRunnable(QObject *parent) : QObject(parent)
{
    m_next = 0;
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

QFAppScriptRunnable *QFAppScriptRunnable::wait(QString type,QJSValue script)
{
    QFAppScriptRunnable* runnable = new QFAppScriptRunnable(this);
    runnable->setType(type);
    runnable->setScript(script);
    setNext(runnable);
    return runnable;
}

QFAppScriptRunnable *QFAppScriptRunnable::next() const
{
    return m_next;
}

void QFAppScriptRunnable::setNext(QFAppScriptRunnable *next)
{
    m_next = next;
}

