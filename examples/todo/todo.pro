TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $$PWD

# If you install QuickFlux by qpm.pri, change it to include(vendor/vendor.pri)
include(../../quickflux.pri)

DISTFILES += \
    README.md
