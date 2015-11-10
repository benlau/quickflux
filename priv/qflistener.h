#ifndef QFLISTENER_H
#define QFLISTENER_H

#include <QObject>
#include <QJSValue>

/// A listener class for AppDispatcher.

class QFListener : public QObject
{
    Q_OBJECT
public:
    explicit QFListener(QObject *parent = 0);
    ~QFListener();

    QJSValue callback() const;

    void setCallback(const QJSValue &callback);

    void dispatch(QString type, QJSValue message);

    int listenerId() const;

    void setListenerId(int listenerId);

signals:
    void dispatched(QString type, QJSValue message);

public slots:

private:
    QJSValue m_callback;
    int m_listenerId;
};

#endif // QFLISTENER_H
