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
public:
    explicit QFAppScript(QQuickItem *parent = 0);

    QQmlScriptString script() const;
    void setScript(const QQmlScriptString &script);

signals:
    void started();
    void finished(int returnCode);
    void scriptChanged();

public slots:
    void exit(int returnCode = 0);
    void run();

    QFAppScriptRunnable* wait(QString type,QJSValue script);

private slots:
    void onDispatched(QString type,QJSValue message);

private:
    virtual void componentComplete();
    void cancel();
    void clear();

    QQmlScriptString m_script;
    QList<QFAppScriptRunnable*> m_runnables;
    QPointer<QFAppDispatcher> m_dispatcher;
    bool m_running;
    bool m_processing;

};

#endif // QFAPPSCRIPT_H
