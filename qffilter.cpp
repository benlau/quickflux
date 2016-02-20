#include <QtCore>
#include <QMetaObject>
#include "qffilter.h"

/*!
   \qmltype Filter
   \brief Add filter rule to AppListener

Filter component listens for the parent's dispatched signal, if a dispatched signal
match with its type, it will emit its own "dispatched" signal. Otherwise, it will
simply ignore the signal.

This component provides an alternative way to filter incoming message which is
suitable for making Store component.

Example:

\code

pragma Singleton
import QtQuick 2.0
import QuickFlux 1.0
import "../actions"

AppListener {
    id: store

    property ListModel model: ListModel { }

    Filter {
        type: ActionTypes.addTask
        onDispatched: {
            model.append({task: message.task});
        }
    }

}

\endcode

It is not suggested to use nested AppListener in a Store component. Because nested AppListener
do not share the same AppListener::listenerId, it will be difficult to control the order
of message reception between store component.

In contrast, Filter share the same listenerId with its parent, and therefore it is a solution
for above problem.

*/

/*!
  \qmlsignal Filter::dispatched(string type, object message)

  It is a proxy of parent's dispatched signal. If the parent emits a signal matched by the Filter::type property,
  it will emit this signal
 */


QFFilter::QFFilter(QObject *parent) : QObject(parent)
{

}

/*! \qmlproperty string Filter::type

  Set a filter to incoming messages. Only message with type matched by the filter will emit "dispatched" signal.
 */


QString QFFilter::type() const
{
    return m_type;
}

void QFFilter::setType(const QString &type)
{
    m_type = type;
    emit typeChanged();
}

void QFFilter::classBegin()
{

}

void QFFilter::componentComplete()
{
    QObject* object = parent();

    if (!object) {
        qDebug() << "Filter - Disabled due to missing parent. ";
        return;
    }

    connect(object,SIGNAL(dispatched(QString,QJSValue)),
            this,SLOT(filter(QString,QJSValue)));

}

void QFFilter::filter(QString type, QJSValue message)
{
    if (type == m_type) {
        emit dispatched(type, message);
    }
}
