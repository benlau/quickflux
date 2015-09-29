#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickView>
#include <QQuickItem>
#include "qfappdispatcher.h"

class AppDispatcherTests : public QObject
{
    Q_OBJECT

public:
    AppDispatcherTests();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void instance();
};

AppDispatcherTests::AppDispatcherTests()
{
}

void AppDispatcherTests::initTestCase()
{
}

void AppDispatcherTests::cleanupTestCase()
{
}

void AppDispatcherTests::instance()
{
    QQmlApplicationEngine engine;

    engine.addImportPath("qrc:/");

    QUrl url("qrc:///dummy.qml");
    engine.load(url);

    QObject *rootItem = engine.rootObjects().first();

    QVERIFY(rootItem);

    QFAppDispatcher* dispatcher = QFAppDispatcher::instance(&engine);
    QVERIFY(dispatcher);

    dispatcher->dispatch("TestMessage");

}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc,argv);
    AppDispatcherTests tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "tst_appdispatchertests.moc"
