#include <QtCore>
#include <QMetaMethod>
#include <QtQml>
#include "qfactioncreator.h"
#include "priv/qfsignalproxy.h"
#include "qfappdispatcher.h"

QFActionCreator::QFActionCreator(QObject *parent) : QObject(parent)
{

}

void QFActionCreator::classBegin()
{

}

void QFActionCreator::componentComplete()
{
    QQmlEngine* engine = qmlEngine(this);
    QFAppDispatcher* dispatcher = QFAppDispatcher::instance(engine);

    const int memberOffset = QObject::staticMetaObject.methodCount();

    const QMetaObject* meta = metaObject();

    int count = meta->methodCount();

    for (int i = memberOffset ; i < count ;i++) {
        QMetaMethod method = meta->method(i);
        if (method.methodType() == QMetaMethod::Signal) {
            QFSignalProxy* proxy = new QFSignalProxy(this);
            proxy->bind(this, i, engine, dispatcher);
        }
    }

}
