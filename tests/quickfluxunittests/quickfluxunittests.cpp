#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickView>
#include <QQuickItem>
#include <QSignalSpy>
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

    QQmlApplicationEngine engine;

    engine.addImportPath("qrc:/");

    QUrl url("qrc:///dummy.qml");
    engine.load(url);

    QFSignalProxy proxy;

    const QMetaObject* meta = metaObject();

    int idx = meta->indexOfMethod("dummySignal(int,int)");

    QFAppDispatcher *dispatcher = QFAppDispatcher::instance(&engine);

    QSignalSpy spy(dispatcher,SIGNAL(dispatched(QString,QJSValue)));
    QVERIFY(spy.count() == 0);

    proxy.bind(this, idx, &engine, dispatcher);

    emit dummySignal(1,999);

    QCOMPARE(spy.count(), 1);

    QVariantList list = spy[0];
    QVERIFY(list.size() == 2);

    QString type = list.at(0).toString();
    QVERIFY(type == "dummySignal");

    QJSValue message = list.at(1).value<QJSValue>();
    QCOMPARE(message.property("v1").toInt(), 1);
    QCOMPARE(message.property("v2").toInt(), 999);

}

