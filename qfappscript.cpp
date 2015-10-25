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
    m_running = false;
    clear();
    emit finished(returnCode);
}

void QFAppScript::run()
{
    if (m_processing) {
        qWarning() << "AppScript::run(): Don't call run() within script / wait callback";
        return;
    }

    m_processing = true;
    clear();

    if (m_dispatcher.isNull()) {
        qWarning() << "AppScript::run() - Missing AppDispatcher. Aborted.";
        m_processing = false;
        return;
    }

    connect(m_dispatcher.data(),SIGNAL(dispatched(QString,QJSValue)),
            this,SLOT(onDispatched(QString,QJSValue)));

    m_running = true;

    emit started();

    QQmlExpression expr(m_script);
    expr.evaluate();
    if (expr.hasError()) {
        qWarning() << expr.error();
    }

    if (m_runnables.size() == 0) {
        exit(0);
    }

    m_processing = false;
}

QFAppScriptRunnable *QFAppScript::wait(QString type, QJSValue script)
{
    QFAppScriptRunnable* runnable = new QFAppScriptRunnable(this);
    runnable->setType(type);
    runnable->setScript(script);
    m_runnables.append(runnable);
    return runnable;
}

void QFAppScript::onDispatched(QString type, QJSValue message)
{
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
}

void QFAppScript::cancel()
{
    exit(-1);
}

void QFAppScript::clear()
{
    if (!m_dispatcher.isNull())
        m_dispatcher->disconnect(this);

    for (int i = 0 ; i < m_runnables.size(); i++) {
        m_runnables[i]->deleteLater();
    }
    m_runnables.clear();
}

QQmlScriptString QFAppScript::script() const
{
    return m_script;
}

void QFAppScript::setScript(const QQmlScriptString &script)
{
    m_script = script;
}

class QFAppScriptRegisterHelper {

public:
    QFAppScriptRegisterHelper() {
        qmlRegisterType<QFAppScript>("QuickFlux", 1, 0, "AppScript");
        qmlRegisterType<QFAppScriptRunnable>();
    }
};

static QFAppScriptRegisterHelper registerHelper;
