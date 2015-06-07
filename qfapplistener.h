#ifndef QFAPPLISTENER_H
#define QFAPPLISTENER_H

#include <QObject>
#include <QJSValue>
#include <QPointer>
#include <QQuickItem>
#include <QMap>

class QFAppListener : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QObject* target READ target WRITE setTarget NOTIFY targetChanged)

public:
    explicit QFAppListener(QQuickItem *parent = 0);
    ~QFAppListener();

    QObject *target() const;
    void setTarget(QObject *target);

    /// Resgister a callback for a message.
    Q_INVOKABLE QFAppListener* on(QString name,QJSValue callback);

signals:
    /// It is emitted whatever it has received a dispatched message from AppDispatcher.
    void received(QString name,QJSValue message);

    void targetChanged();

public slots:

private:

    Q_INVOKABLE void onDispatcherReceived(QString name,QJSValue message);

    QPointer<QObject> m_target;

    QMap<QString,QList<QJSValue> >  mapping;
};

#endif // QFAPPLISTENER_H
