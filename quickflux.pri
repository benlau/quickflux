INCLUDEPATH += $$PWD

DISTFILES += \
    $$PWD/README.md

HEADERS += \
    $$PWD/qfappdispatcher.h \
    $$PWD/qfapplistener.h \
    $$PWD/qfappscript.h \
    $$PWD/priv/qfappscriptrunnable.h \
    $$PWD/priv/qfappscriptdispatcherwrapper.h

SOURCES += \
    $$PWD/qfapplistener.cpp \
    $$PWD/qfappdispatcher.cpp \
    $$PWD/qfappscript.cpp \
    $$PWD/qfappscriptrunnable.cpp \
    $$PWD/qfappscriptdispatcherwrapper.cpp
