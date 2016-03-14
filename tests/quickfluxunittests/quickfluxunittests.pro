QT += core
QT -= gui

CONFIG += c++11

TARGET = quickfluxunittests
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    quickfluxunittests.cpp

include(vendor/vendor.pri)
include(../../quickflux.pri)

HEADERS += \
    quickfluxunittests.h

RESOURCES += \
    qml.qrc
