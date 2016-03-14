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
#include "quickfluxunittests.h"

QuickFluxUnitTests::QuickFluxUnitTests()
{
}

void QuickFluxUnitTests::initTestCase()
{
}

void QuickFluxUnitTests::cleanupTestCase()
{
}

void QuickFluxUnitTests::instance()
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

void QuickFluxUnitTests::singletonObject()
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

