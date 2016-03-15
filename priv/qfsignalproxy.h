#ifndef QFSIGNALPROXY_H
#define QFSIGNALPROXY_H

#include <QObject>
#include <QVariant>
#include <QQmlEngine>
#include <QVector>
#include <QPointer>
#include "qfappdispatcher.h"

class QFSignalProxy : public QObject
{
public:
    explicit QFSignalProxy(QObject *parent = 0);

    void bind(QObject* source, int signalIdx, QQmlEngine* engine, QFAppDispatcher* dispatcher);

    int qt_metacall(QMetaObject::Call _c, int _id, void **_a);

private:
    void dispatch(const QVariantMap &message);

    QString type;
    QVector<int> parameterTypes;
    QVector<QString> parameterNames;
    QPointer<QQmlEngine> m_engine;
    QPointer<QFAppDispatcher> m_dispatcher;

};

#endif // QFSIGNALPROXY_H
