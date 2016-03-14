#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickView>
#include <QQuickItem>
#include "automator.h"
#include "qfappdispatcher.h"
#include "quickfluxunittests.h"
#include "priv/qfsignalproxy.h"

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

void QuickFluxUnitTests::signalProxy()
{
    QFSignalProxy proxy;

    const QMetaObject* meta = metaObject();

    int idx = meta->indexOfMethod("dummySignal(int,int)");

    proxy.bind(this, idx);

    emit dummySignal(1,999);


}

