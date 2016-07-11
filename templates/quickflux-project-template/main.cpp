#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "appview.h"

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    Q_UNUSED(app);

    AppView view;

    return view.exec();
}
