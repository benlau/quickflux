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
    tst_actiondispatcher.qml \
    tst_appdispatcher_dispatch_sequence.qml

