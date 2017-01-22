#ifndef QFSTORE_H
#define QFSTORE_H

#include <QObject>
#include <QQmlListProperty>
#include <QJSValue>
#include <QPointer>
#include <QQmlParserStatus>
#include "qfactioncreator.h"
#include "qfdispatcher.h"

class QFStore : public QObject, public QQmlParserStatus
{
    Q_OBJECT
public:
    explicit QFStore(QObject *parent = 0);
    Q_PROPERTY(QObject* bindSource READ bindSource WRITE setBindSource NOTIFY bindSourceChanged)
    Q_PROPERTY(QQmlListProperty<QObject> children READ children)
    Q_CLASSINFO("DefaultProperty", "children")

    QQmlListProperty<QObject> children();

    QObject* bindSource() const;
    void setBindSource(QObject* source);

signals:
    void dispatched(QString type, QJSValue message);

    void bindSourceChanged();

public slots:
    void dispatch(QString type, QJSValue message = QJSValue());

    void bind(QObject* source);


protected:
    void classBegin();
    void componentComplete();

private slots:
    void setup();

private:
    QObjectList m_children;

    QPointer<QObject> m_bindSource;

    QPointer<QFActionCreator> m_actionCreator;
    QPointer<QFDispatcher> m_dispatcher;

};

#endif // QFSTORE_H
