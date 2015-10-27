#include <QtQml>
#include <QVariant>
#include <QPointer>
#include <QQmlExpression>
#include <QtCore>
#include "qfappscript.h"

QFAppScript::QFAppScript(QQuickItem *parent) : QQuickItem(parent)
{
    m_running = false;
    m_processing = false;
}

void QFAppScript::exit(int returnCode)
{
    clear();
    setRunning(false);
    emit finished(returnCode);
}

void QFAppScript::run(QJSValue message)
{
    if (m_processing) {
        qWarning() << "AppScript::run(): Don't call run() within script / wait callback";
        return;
    }

    m_processing = true;
    clear();
    setMessage(message);

    if (m_dispatcher.isNull()) {
        qWarning() << "AppScript::run() - Missing AppDispatcher. Aborted.";
        m_processing = false;
        return;
    }

    setRunning(true);

    emit started();

    QQmlExpression expr(m_script);

    if (!m_script.isEmpty()) {
        expr.evaluate();
    }

    if (expr.hasError()) {
        qWarning() << expr.error();
    }

    if (m_runnables.size() == 0) {
        exit(0);
    }

    m_processing = false;
}

QFAppScriptRunnable *QFAppScript::wait(QJSValue condition, QJSValue script)
{
    QFAppScriptRunnable* runnable = new QFAppScriptRunnable(this);
    runnable->setEngine(qmlEngine(this));
    runnable->setCondition(condition);
    runnable->setScript(script);
    m_runnables.append(runnable);
    return runnable;
}

void QFAppScript::onDispatched(QString type, QJSValue message)
{
    if (!m_runWhen.isEmpty() &&
        type == m_runWhen &&
        !m_processing) {

        if (m_running) {
            abort();
        }
        run(message);
        return;
    }

    if (!m_running) {
        return;
    }

    m_processing = true;
    QList<int> marked;
    for (int i = 0 ; i < m_runnables.size() ; i++) {
        if (m_runnables[i]->type() == type) {
            m_runnables[i]->run(message);
            marked << i;
        }
    }

    if (!m_running) {
        // Terminate if exit() is called in runnable
        m_processing = false;
        return;
    }

    for (int i = marked.size() - 1 ; i >= 0 ; i--) {
        int idx = marked[i];
        QFAppScriptRunnable* runnable = m_runnables.takeAt(idx);

        QFAppScriptRunnable* next = runnable->next();
        if (next) {
            next->setParent(this);
            m_runnables.append(next);
        }
        runnable->release();
        runnable->deleteLater();
    }

    m_processing = false;

    // All the tasks are finished
    if (m_runnables.size() == 0) {
        exit(0);
    }
}

void QFAppScript::componentComplete()
{
    QQuickItem::componentComplete();

    QQmlEngine *engine = qmlEngine(this);
    Q_ASSERT(engine);

    m_dispatcher = qobject_cast<QFAppDispatcher*>(QFAppDispatcher::instance(engine));

    connect(m_dispatcher.data(),SIGNAL(dispatched(QString,QJSValue)),
            this,SLOT(onDispatched(QString,QJSValue)));
}

void QFAppScript::abort()
{
    exit(-1);
}

void QFAppScript::clear()
{
    for (int i = 0 ; i < m_runnables.size(); i++) {
        m_runnables[i]->deleteLater();
    }
    m_runnables.clear();
}

bool QFAppScript::running() const
{
    return m_running;
}

void QFAppScript::setRunning(bool running)
{
    if (m_running == running) {
        return;
    }
    m_running = running;
    emit runningChanged();
}

QJSValue QFAppScript::message() const
{
    return m_message;
}

void QFAppScript::setMessage(const QJSValue &message)
{
    m_message = message;
    emit messageChanged();
}

QString QFAppScript::runWhen() const
{
    return m_runWhen;
}

void QFAppScript::setRunWhen(const QString &runWhen)
{
    m_runWhen = runWhen;
    emit runWhenChanged();
}

QQmlScriptString QFAppScript::script() const
{
    return m_script;
}

void QFAppScript::setScript(const QQmlScriptString &script)
{
    m_script = script;
    emit scriptChanged();
}

class QFAppScriptRegisterHelper {

public:
    QFAppScriptRegisterHelper() {
        qmlRegisterType<QFAppScript>("QuickFlux", 1, 0, "AppScript");
        qmlRegisterType<QFAppScriptRunnable>();
    }
};

static QFAppScriptRegisterHelper registerHelper;
