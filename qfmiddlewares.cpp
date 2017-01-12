#include <QQmlExpression>
#include <QQmlEngine>
#include <QQmlContext>
#include "qfmiddlewares.h"

QFMiddlewares::QFMiddlewares()
{
    m_engine = 0;
}

void QFMiddlewares::classBegin()
{

}

void QFMiddlewares::componentComplete()
{
    m_engine = qmlEngine(this);
    registerNext();
}

void QFMiddlewares::registerNext()
{

    QString script = "function (middlewares) {"
                     "  function create(index) {"
                     "    return function (type, message) {"
                     "      middlewares.next(index + 1, type , message);"
                     "    }"
                     "  }"
                     "  var data = middlewares.data;"
                     "  for (var i = 0 ; i < data.length; i++) {"
                     "    var m = data[i];"
                     "    m.next = create(i);"
                     "  }"
                     "}";

    QJSValue creator = m_engine->evaluate(script);

    QJSValue middlewares = m_engine->newQObject(this);
    QJSValueList args;
    args << middlewares;
    QJSValue ret = creator.call(args);

    if (ret.isError()) {
        QString message = QString("%1:%2: %3: %4")
                          .arg(ret.property("fileName").toString())
                          .arg(ret.property("lineNumber").toString())
                          .arg(ret.property("name").toString())
                          .arg(ret.property("message").toString());
        qWarning() << message;
    }
}
