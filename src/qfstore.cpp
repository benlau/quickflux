#include <QtQml>
#include <QQmlEngine>
#include <QFAppDispatcher>
#include "priv/quickfluxfunctions.h"
#include "qfactioncreator.h"
#include "qfstore.h"

/*!
   \qmltype Store
   \inqmlmodule QuickFlux 1.1
   \brief Store Component

    Store is a helper item for implement the data “Store” component in a Quick Flux application. It could listen from ActionCreator / Dispatcher    component and redispatch the received action to another store components (e.g children store).

    It is a replacement of AppListener component

    Example:

    \code

    import QuickFlux 1.1

    Store {
      bindSource: AppDispatcher

      Filter {
        type: ActionTypes.addItem
        onDispatched: {
          /// Process
        }
      }

    }

    \endcode

    \b{The order of action delivery:}

    \code

    Store {
      id: rootStore

      bindSource: AppDispatcher

      property alias page1 : page1

      Store {
        id: page1
      }

      Store {
        id: page2
      }

      Filter {
        id: filter1
      }

    }
    \endcode

In the example above, the rootStore is bind to AppDispatcher, whatever the dispatcher dispatch an action, it will first re-dispatch the action to its children sequentially. Then emit the dispatched signal on itself. Therefore, the order of receivers is: page1, page2 then filter1.

If the redispatchTargets property is set, Store component will also dispatch the received action to the listed objects.

*/

/*!
  \qmlsignal Store::dispatched(string type, object message)

  This signal is emitted when a message is received by this store.

  There has two suggested methods to listen this signal:

  Method 1 - Use Filter component

  \code

  Store {
      Filter {
          type: ActionTypes.addItem
          onDispatched: {
              // process here
          }
      }
  }
  \endcode

  Method 2 - Use filter function

  \code
  Store {
      filterFunctionEnabled: true

      function addItem(message) {

      }
  }
  \endcode

*/

/*! \qmlproperty bool Store::filterFunctionEnabled
If this property is true, whatever the store component received a new action. Beside to emit a dispatched signal, it will search for a function with a name as the action. If it exists, it will call also call the function.

\code

Store {
  filterFunctionEnabled: true

  function addItem(message) {
  }
}
\endcode

The default value is false
 */


QFStore::QFStore(QObject *parent) : QObject(parent) , m_filterFunctionEnabled(false)
{

}

QQmlListProperty<QObject> QFStore::children()
{
    #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return QQmlListProperty<QObject>(qobject_cast<QObject*>(this), &m_children);
    #else
    return QQmlListProperty<QObject>(qobject_cast<QObject*>(this), m_children);
    #endif
}

void QFStore::dispatch(QString type, QJSValue message)
{
    QQmlEngine* engine = qmlEngine(this);
    QF_PRECHECK_DISPATCH(engine, type, message);

    foreach(QObject* child , m_children) {
        QFStore* store = qobject_cast<QFStore*>(child);
        if (!store) {
            continue;
        }
        store->dispatch(type, message);
    }

    foreach(QObject* child , m_redispatchTargets) {
        QFStore* store = qobject_cast<QFStore*>(child);

        if (!store) {
            continue;
        }
        store->dispatch(type, message);
    }

    if (m_filterFunctionEnabled) {
        const QMetaObject *meta = metaObject();
        QByteArray signature;
        int index;

        signature = QMetaObject::normalizedSignature(QString("%1(QVariant)").arg(type).toUtf8().constData());
        if ( (index = meta->indexOfMethod(signature.constData())) >= 0 ) {
            QMetaMethod method = meta->method(index);
            QVariant value = QVariant::fromValue<QJSValue>(message);

            method.invoke(this,Qt::DirectConnection, Q_ARG(QVariant, value));
        }

        signature = QMetaObject::normalizedSignature(QString("%1()").arg(type).toUtf8().constData());
        if ( (index = meta->indexOfMethod(signature.constData())) >= 0 ) {
            QMetaMethod method = meta->method(index);

            method.invoke(this);
        }
    }

    emit dispatched(type, message);
}

void QFStore::bind(QObject *source)
{
    setBindSource(source);
}

void QFStore::setup()
{
    QFActionCreator *creator = 0;
    QFDispatcher* dispatcher = 0;

    creator = qobject_cast<QFActionCreator*>(m_bindSource.data());

    if (creator) {
        dispatcher = creator->dispatcher();
    } else {
        dispatcher = qobject_cast<QFDispatcher*>(m_bindSource.data());
    }

    if (m_actionCreator.data() == creator &&
        m_dispatcher.data() == dispatcher) {
        // Nothing changed.
        return;
    }

    if (!m_actionCreator.isNull() &&
        m_actionCreator.data() != creator) {
        m_actionCreator->disconnect(this);
    }

    if (!m_dispatcher.isNull() &&
        m_dispatcher.data() != dispatcher) {
        m_dispatcher->disconnect(this);
    }

    m_actionCreator = creator;
    m_dispatcher = dispatcher;

    if (!m_actionCreator.isNull()) {
        connect(m_actionCreator.data(),SIGNAL(dispatcherChanged()),
                this,SLOT(setup()));
    }

    if (!m_dispatcher.isNull()) {
        connect(dispatcher,SIGNAL(dispatched(QString,QJSValue)),
                this,SLOT(dispatch(QString,QJSValue)));
    }

}

/*! \qmlproperty array Store::redispatchTargets

  By default, the Store component redispatch the received action to its children sequentially. If this property is set,
  the action will be re-dispatch to the target objects too.

  \code

    Store {
        id: bridgeStore

        redispatchTargets: [
            SingletonStore1,
            SingletonStore2
        ]
    }
  \endcode


 */

QQmlListProperty<QObject> QFStore::redispatchTargets()
{
    #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return QQmlListProperty<QObject>(qobject_cast<QObject*>(this), &m_redispatchTargets);
    #else
    return QQmlListProperty<QObject>(qobject_cast<QObject*>(this), m_redispatchTargets);
    #endif
}


/*! \qmlproperty object Store::bindSource
 *
 * This property holds the source of action. It can be an ActionCreator / Dispatcher component
 *
 * The default value is null, and it listens to AppDispatcher
 */


QObject *QFStore::bindSource() const
{
    return m_bindSource.data();
}

void QFStore::setBindSource(QObject *source)
{
    m_bindSource = source;
    setup();
    emit bindSourceChanged();
}
