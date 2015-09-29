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
    void singletonObject();
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

void AppDispatcherTests::singletonObject()
{
    QQmlApplicationEngine engine;

    engine.addImportPath("qrc:/");

    QUrl url("qrc:///dummy.qml");
    engine.load(url);

    QObject *rootItem = engine.rootObjects().first();

    QVERIFY(rootItem);

    QObject* dummyAction = QFAppDispatcher::singletonObject(&engine,"QuickFluxTests",3,2,"DummyAction");

    QVERIFY(dummyAction);
    QVERIFY(dummyAction->property("value").toInt() == 13);
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc,argv);
    AppDispatcherTests tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "tst_appdispatchertests.moc"
