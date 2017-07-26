cmake_minimum_required(VERSION 3.8.2)

project(middleware)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)

include_directories(${Qt5Qml_INCLUDE_DIRS})
include_directories(${Qt5Gui_INCLUDE_DIRS})

#include("${CMAKE_CURRENT_SOURCE_DIR}/../../CMakeLists.txt")

# Generate rules for building source files from the resources
qt5_add_resources(QRCS qml.qrc)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../ ${CMAKE_CURRENT_BINARY_DIR}/quickflux)

message(STATUS "test" ${quickflux_SOURCES})

# Tell CMake to create the middleware executable
add_executable(middleware main.cpp ${QRCS} quickflux)

#target_link_libraries(middleware ${CMAKE_CURRENT_BINARY_DIR}/quickflux/libquickflux.a)
link_directories(quickflux)
target_link_libraries(middleware  quickflux)

qt5_use_modules(middleware Qml Gui)

