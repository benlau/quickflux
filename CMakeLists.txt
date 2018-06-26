#
# To build it with cmake, you should register qml types explicitly by calling registerQuickFluxQmlTypes() in your main.cpp
# See examples/middleware for example
#

cmake_minimum_required(VERSION 3.0.0)
project(quickflux)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_DEBUG_POSTFIX d)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

find_package(Qt5Core REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Gui REQUIRED)

include_directories(
	${SRC_DIR}
	${Qt5Qml_INCLUDE_DIRS}
)

add_definitions(${Qt5Qml_DEFINITIONS})
add_definitions(${Qt5Quick_DEFINITIONS})

#Include headers from priv folder as CMAKE_AUTOMOC do not works on it
qt5_wrap_cpp(moc 
    ${SRC_DIR}/priv/qflistener.h
    ${SRC_DIR}/priv/qfappscriptrunnable.h
    ${SRC_DIR}/priv/qfappscriptdispatcherwrapper.h
)

set(quickflux_SOURCES
    ${SRC_DIR}/qfapplistener.cpp
    ${SRC_DIR}/qfappdispatcher.cpp
    ${SRC_DIR}/qfappscript.cpp
    ${SRC_DIR}/qfappscriptrunnable.cpp
    ${SRC_DIR}/qfappscriptdispatcherwrapper.cpp
    ${SRC_DIR}/qflistener.cpp
    ${SRC_DIR}/qfqmltypes.cpp
    ${SRC_DIR}/qfapplistenergroup.cpp
    ${SRC_DIR}/qfappscriptgroup.cpp
    ${SRC_DIR}/qffilter.cpp
    ${SRC_DIR}/qfkeytable.cpp
    ${SRC_DIR}/priv/qfsignalproxy.cpp
    ${SRC_DIR}/qfactioncreator.cpp
    ${SRC_DIR}/qfmiddleware.cpp
    ${SRC_DIR}/qfmiddlewarelist.cpp
    ${SRC_DIR}/qfobject.cpp
    ${SRC_DIR}/qfstore.cpp
    ${SRC_DIR}/qfdispatcher.cpp
    ${SRC_DIR}/priv/qfmiddlewareshook.cpp
    ${SRC_DIR}/priv/qfhook.cpp
    ${SRC_DIR}/priv/quickfluxfunctions.cpp
    ${SRC_DIR}/qfhydrate.cpp
)

set(quickflux_PRIVATE_HEADERS
    ${SRC_DIR}/priv/qfappscriptrunnable.h
    ${SRC_DIR}/priv/qfappscriptdispatcherwrapper.h
    ${SRC_DIR}/priv/qflistener.h
    ${SRC_DIR}/priv/qfsignalproxy.h
)

set(quickflux_PUBLIC_HEADERS
    ${SRC_DIR}/qfappdispatcher.h
    ${SRC_DIR}/qfapplistener.h
    ${SRC_DIR}/qfappscript.h
    ${SRC_DIR}/qfapplistenergroup.h
    ${SRC_DIR}/qfappscriptgroup.h
    ${SRC_DIR}/qffilter.h
    ${SRC_DIR}/qfkeytable.h
    ${SRC_DIR}/qfactioncreator.h
    ${SRC_DIR}/qfmiddleware.h
    ${SRC_DIR}/qfmiddlewarelist.h
    ${SRC_DIR}/qfdispatcher.h
    ${SRC_DIR}/QFAppDispatcher
    ${SRC_DIR}/QFKeyTable
    ${SRC_DIR}/QuickFlux
)

add_library(quickflux STATIC
	${quickflux_SOURCES}
	${quickflux_PRIVATE_HEADERS}
	${quickflux_PUBLIC_HEADERS}
	${moc}
)
target_link_libraries(quickflux
	Qt5::Qml
	Qt5::Quick
	Qt5::Core
)

target_include_directories(quickflux PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS quickflux
	DESTINATION lib
)

install(FILES
	${quickflux_PUBLIC_HEADERS}
	DESTINATION includes
)

install(FILES
	${quickflux_PRIVATE_HEADERS}
	DESTINATION includes/priv
)
