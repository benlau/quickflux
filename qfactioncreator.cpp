#include <QtCore>
#include <QMetaMethod>
#include <QtQml>
#include "qfactioncreator.h"
#include "priv/qfsignalproxy.h"
#include "qfappdispatcher.h"

/*!
  \qmltype ActionCreator
  \brief Create message from signal then dispatch via AppDispatcher

ActionCreator is a component that listens on its own signals, convert to message then dispatch via AppDispatcher. The message type will be same as the signal name. There has no limitation on number of arguments and their data type.

For example, you may declare an ActionCreator based component as:

\code
import QtQuick 2.0
import QuickFlux 1.0
pragma singleton

ActionCreator {
   signal open(string url);
}
\endcode

It is equivalent to:

\code
import QtQuick 2.0
import QuickFlux 1.0
pragma singleton

Item {
   function open(url) {
     AppDispatcher.dispatch(“open”, {url: url});
   }
}
\endcode

 */

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
