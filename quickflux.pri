INCLUDEPATH += $$PWD

DISTFILES += \
    $$PWD/README.md

HEADERS += \
    $$PWD/qfappdispatcher.h \
    $$PWD/qfapplistener.h \
    $$PWD/qfappscript.h \
    $$PWD/priv/qfappscriptrunnable.h \
    $$PWD/priv/qfappscriptdispatcherwrapper.h \
    $$PWD/priv/qflistener.h \
    $$PWD/qfapplistenergroup.h \
    $$PWD/qfappscriptgroup.h \
    $$PWD/qffilter.h \
    $$PWD/qfkeytable.h

SOURCES += \
    $$PWD/qfapplistener.cpp \
    $$PWD/qfappdispatcher.cpp \
    $$PWD/qfappscript.cpp \
    $$PWD/qfappscriptrunnable.cpp \
    $$PWD/qfappscriptdispatcherwrapper.cpp \
    $$PWD/qflistener.cpp \
    $$PWD/qfqmltypes.cpp \
    $$PWD/qfapplistenergroup.cpp \
    $$PWD/qfappscriptgroup.cpp \
    $$PWD/qffilter.cpp \
    $$PWD/qfkeytable.cpp
