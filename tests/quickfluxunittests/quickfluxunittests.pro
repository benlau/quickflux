QT += core
QT -= gui

CONFIG += c++11

TARGET = quickfluxunittests
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += main.cpp \
    quickfluxunittests.cpp \
    testenv.cpp \
    actiontypes.cpp

include(vendor/vendor.pri)
include(../../quickflux.pri)

HEADERS += \
    quickfluxunittests.h \
    testenv.h \
    actiontypes.h

RESOURCES += \
    qml.qrc

DISTFILES += \
    qmltests/tst_appdispatcher_dispatch_reentrant.qml \
    qmltests/tst_appdispatcher_waitfor.qml \
    qmltests/tst_appdispatcher.qml \
    qmltests/tst_applistener_alwayson.qml \
    qmltests/tst_applistener_filter.qml \
    qmltests/tst_applistener_waitfor.qml \
    qmltests/tst_applistener.qml \
    qmltests/tst_applistenergroup.qml \
    qmltests/tst_appscript.qml \
    qmltests/tst_appscriptgroup.qml \
    qmltests/tst_filter.qml \
    qmltests/tst_keytable.qml \
    qmltests/tst_qimage.qml \
    qmltests/tst_actioncreator.qml
