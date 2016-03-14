#include <QtCore>
#include <QMetaObject>
#include "qfsignalproxy.h"

QFSignalProxy::QFSignalProxy(QObject *parent) : QObject(parent)
{

}

void QFSignalProxy::bind(QObject *source, int signalIdx)
{
    const int memberOffset = QObject::staticMetaObject.methodCount();

    QMetaMethod method = source->metaObject()->method(signalIdx);

    types = QVector<int>(method.parameterCount());
    names = QVector<QString>(method.parameterCount());

    for (int i = 0 ; i < method.parameterCount() ; i++) {
        types[i] = method.parameterType(i);
        names[i] = QString(method.parameterNames().at(i));
    }

    if (!QMetaObject::connect(source, signalIdx, this, memberOffset, Qt::AutoConnection, 0)) {
        qWarning() << "Failed to bind signal";
    }
}

int QFSignalProxy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    int methodId = QObject::qt_metacall(_c, _id, _a);

    if (methodId < 0) {
        return methodId;
    }

    if (_c == QMetaObject::InvokeMetaMethod) {

        if (methodId == 0) {
            QVariantMap message;

            for (int i = 0 ; i < types.count() ; i++) {
                const QMetaType::Type type = static_cast<QMetaType::Type>(types.at(i));
                QVariant v;

                if (type == QMetaType::QVariant) {
                    v = *reinterpret_cast<QVariant *>(_a[i + 1]);
                } else {
                    v = QVariant(type, _a[i + 1]);
                }

                message[names.at(i)] = v;
            }
            //@TODO - Dispatch message
        }
        methodId--;
    }

    return methodId;
}
