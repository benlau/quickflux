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

signals:

public slots:

private:
    QJSValue m_script;
    QString m_type;

};

#endif // QFAPPSCRIPTRUNNABLE_H
