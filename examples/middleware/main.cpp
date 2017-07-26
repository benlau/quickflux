#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QuickFlux>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // It is needed only if you link QuickFlux from cmake via libquickflux.a. Build with qpm/quickflux.pri is not needed.
    registerQuickFluxQmlTypes();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

