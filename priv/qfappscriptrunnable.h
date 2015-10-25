#ifndef QFAPPSCRIPTRUNNABLE_H
#define QFAPPSCRIPTRUNNABLE_H

#include <QObject>
#include <QQmlScriptString>
#include <QJSValue>

class QFAppScriptRunnable : public QObject
{
    Q_OBJECT
public:
    explicit QFAppScriptRunnable(QObject *parent = 0);

    QJSValue script() const;
    void setScript(const QJSValue &script);

    QString type() const;
    void setType(const QString &type);

    void run(QJSValue message);

    QFAppScriptRunnable *next() const;
    void setNext(QFAppScriptRunnable *next);

signals:

public slots:
    QFAppScriptRunnable* wait(QString type,QJSValue value);

private:
    QJSValue m_script;
    QString m_type;
    QFAppScriptRunnable* m_next;

};

#endif // QFAPPSCRIPTRUNNABLE_H
