QT       += core
QT       += testlib gui quick qml qmltest

TARGET = qmlunittests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

include(../../quickflux.pri)

DEFINES += SRCDIR=\\\"$$PWD/\\\" BASEDIR=\\\"$$PWD/..\\\"

DISTFILES += \
    tst_appdispatcher_dispatch_reentrant.qml \
    tst_appdispatcher.qml \
    tst_applistener.qml \
    tst_applistener_filter.qml \
    tst_applistener_alwayson.qml

