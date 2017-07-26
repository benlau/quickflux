#
# To build it with cmake, you should register qml types explicitly by calling registerQuickFluxQmlTypes() in your main.cpp
# See examples/middleware for example
#

cmake_minimum_required(VERSION 3.0.0)
project(quickflux)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Gui REQUIRED)

include_directories(${Qt5Qml_INCLUDE_DIRS})

add_definitions(${Qt5Qml_DEFINITIONS})
add_definitions(${Qt5Quick_DEFINITIONS})

#Include headers from priv folder as CMAKE_AUTOMOC do not works on it
qt5_wrap_cpp(moc 
    priv/qflistener.h
    priv/qfappscriptrunnable.h
    priv/qfappscriptdispatcherwrapper.h
)

set(SOURCES
    qfapplistener.cpp
    qfappdispatcher.cpp
    qfappscript.cpp
    qfappscriptrunnable.cpp
    qfappscriptdispatcherwrapper.cpp
    qflistener.cpp
    qfqmltypes.cpp
    qfapplistenergroup.cpp
    qfappscriptgroup.cpp
    qffilter.cpp
    qfkeytable.cpp
    priv/qfsignalproxy.cpp
    qfactioncreator.cpp
    qfmiddleware.cpp
    qfmiddlewarelist.cpp
    qfobject.cpp
    qfstore.cpp
    qfdispatcher.cpp
    priv/qfmiddlewareshook.cpp
    priv/qfhook.cpp
    priv/quickfluxfunctions.cpp
    qfhydrate.cpp
)

set(HEADERS
    qfappdispatcher.h
    qfapplistener.h
    qfappscript.h
    priv/qfappscriptrunnable.h
    priv/qfappscriptdispatcherwrapper.h
    priv/qflistener.h
    qfapplistenergroup.h
    qfappscriptgroup.h
    qffilter.h
    qfkeytable.h
    priv/qfsignalproxy.h
    qfactioncreator.h
    qfmiddleware.h
    qfmiddlewarelist.h
    qfdispatcher.h
    QFAppDispatcher
    QFKeyTable
    QuickFlux
)


add_library(quickflux STATIC ${SOURCES} ${moc})

target_link_libraries(quickflux Qt5::Qml Qt5::Quick Qt5::Core)
target_include_directories(quickflux PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
