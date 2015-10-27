#ifndef QFAPPSCRIPT_H
#define QFAPPSCRIPT_H

#include <QObject>
#include <QJSValue>
#include <QQmlScriptString>
#include <QQuickItem>
#include <QQmlParserStatus>
#include <QPointer>
#include "qfappdispatcher.h"
#include "priv/qfappscriptrunnable.h"

class QFAppScript : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQmlScriptString script READ script WRITE setScript NOTIFY scriptChanged)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(QString runWhen READ runWhen WRITE setRunWhen NOTIFY runWhenChanged)
    Q_PROPERTY(QJSValue message READ message NOTIFY messageChanged)

public:
    explicit QFAppScript(QQuickItem *parent = 0);

    QQmlScriptString script() const;
    void setScript(const QQmlScriptString &script);

    bool running() const;

    QString runWhen() const;
    void setRunWhen(const QString &runWhen);

    QJSValue message() const;
    void setMessage(const QJSValue &message);

signals:
    void started();
    void finished(int returnCode);

    void scriptChanged();
    void runningChanged();
    void runWhenChanged();
    void messageChanged();

public slots:
    void exit(int returnCode = 0);
    void run(QJSValue message = QJSValue());

    QFAppScriptRunnable* once(QJSValue condition, QJSValue script);

private slots:
    void onDispatched(QString type,QJSValue message);

private:
    virtual void componentComplete();
    void abort();
    void clear();
    void setRunning(bool running);

    QQmlScriptString m_script;
    QList<QFAppScriptRunnable*> m_runnables;
    QPointer<QFAppDispatcher> m_dispatcher;
    QString m_runWhen;

    bool m_running;
    bool m_processing;

    // The message object passed to run()
    QJSValue m_message;
};

#endif // QFAPPSCRIPT_H
