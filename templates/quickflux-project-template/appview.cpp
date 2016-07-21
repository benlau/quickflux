#include <QCoreApplication>
#include <QFAppDispatcher>
#include <QQmlContext>
#include <QDir>
#include <QStandardPaths>
#include "appview.h"

AppView::AppView(QObject *parent) : QObject(parent)
{

}

int AppView::exec()
{
    m_engine.addImportPath("qrc:///");
    m_engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QFAppDispatcher* dispatcher = QFAppDispatcher::instance(&m_engine);
    connect(dispatcher,SIGNAL(dispatched(QString,QJSValue)),
            this,SLOT(onDispatched(QString,QJSValue)));

    dispatcher->dispatch("startApp");

    QCoreApplication* app = QCoreApplication::instance();
    return app->exec();
}

void AppView::onDispatched(QString type, QJSValue message)
{
    Q_UNUSED(type);
    Q_UNUSED(message);
}
