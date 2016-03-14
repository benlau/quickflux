#ifndef QFSIGNALPROXY_H
#define QFSIGNALPROXY_H

#include <QObject>
#include <QVariant>

class QFSignalProxy : public QObject
{
public:
    explicit QFSignalProxy(QObject *parent = 0);

    void bind(QObject* source, int signalIdx);

    int qt_metacall(QMetaObject::Call _c, int _id, void **_a);

private:
    QVector<int> types;
    QVector<QString> names;

};

#endif // QFSIGNALPROXY_H
