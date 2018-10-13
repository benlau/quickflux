#include <QQmlEngine>
#include "qfmiddleware.h"
#include "priv/quickfluxfunctions.h"


/*!
   \qmltype Middleware
   \inqmlmodule QuickFlux

\code
import QuickFlux 1.1
\endcode

The middleware in Quick Flux is similar to the one in Redux and those from server libraries like Express and Koa. It is some code you can put between the Dispatcher and Stores. It could modify/defer/remove received actions and insert new actions that will dispatch to Store components. Users may use it for logging, crash reporting, talking to asynchronous components like FileDialog / Camera etc. So that Store components remain “pure”, it holds application logic only and always return the same result for the same input. It is easier to write test cases.


Event Flow

\image middleware-data-flow.png "Concept"

\e{Example Code}

\code
// Action Logger
import QuickFlux 1.1

Middleware {
  function dispatch(type, message) {
    console.log(type, JSON.string(message));
    next(type, message); // propagate the action to next Middleware or Store component type. If you don’t call it, the action will be dropped.
  }
}
\endcode

Whatever the middleware received a new action, it will invoke the "dispatch" function written by user.
If the middleare accept the action, it should call the next() to propagate the action to anothter middleware.
User may modify/insert/delay or remove the action.

\code
// Confirmation Dialog

import QuickFlux 1.1
import QtQuick.Dialogs 1.2

Middleware {

  FileDialog {
    id: fileDialog
    onAccepted: {
      next(ActionTypes.removeItem);
    }
  }

  function dispatch(type, message) {

    if (type === ActionTypes.removeItem) {
      fileDialog.open();
    } else {
      next(type, message);
    }
  }
}
\endcode
*/

QFMiddleware::QFMiddleware(QQuickItem* parent) : QQuickItem(parent),  m_filterFunctionEnabled(false)
{

}

/*! \qmlmethod Middleware::next(string type, object message)

  Pass an action message to next middleware. If it is already the last middleware, the action will be dispatched to Store component.
  */

/*! \qmlproperty bool Middleware::filterFunctionEnabled

If this property is true, whatever the middleware component received a new action. Beside to invoke a dispatch signal, it will search for a function with a name as the action. If it exists, it will call also call the function.

\code

Middleware {
  filterFunctionEnabled: true

  function addItem(message) {
    //process
    next("addItem", message);
  }
}
\endcode

The default value is false
 */
void QFMiddleware::next(QString type, QJSValue message)
{
    QQmlEngine* engine = qmlEngine(this);
    QF_PRECHECK_DISPATCH(engine, type, message);

    if (m_nextCallback.isCallable()) {
        QJSValueList args;
        args << type;
        args << message;
        QJSValue result = m_nextCallback.call(args);
        if (result.isError()) {
            QuickFlux::printException(result);
        }
    }
}

QJSValue QFMiddleware::nextCallback() const
{
    return m_nextCallback;
}

void QFMiddleware::setNextCallback(const QJSValue &value)
{
    m_nextCallback = value;
    emit _nextCallbackChanged();
}
