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

    /// Add a listener to the end of the listeners array for the specified message.  Multiple calls passing the same combination of event and listener will result in the listener being added multiple times.
    Q_INVOKABLE QFAppListener* on(QString name,QJSValue callback);

    /// Remove a listener from the listener array for the specified message.
    Q_INVOKABLE void removeListener(QString name,QJSValue callback);

    Q_INVOKABLE void removeAllListener(QString name = QString());

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
