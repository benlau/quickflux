#-------------------------------------------------
#
# Project created by QtCreator 2015-09-29T10:35:39
#
#-------------------------------------------------

QT       += testlib qml quick

QT       -= gui

TARGET =  cppunittests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_appdispatchertests.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../../quickflux.pri)

RESOURCES += \
    qml.qrc
