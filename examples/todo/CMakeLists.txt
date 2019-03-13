cmake_minimum_required(VERSION 3.1.0)

project(todo)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTORCC ON)

find_package(Qt5Qml CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Quick CONFIG REQUIRED)

set(todo_SRCS
  main.cpp
  qml.qrc
  )

include(ExternalProject)

ExternalProject_Add(QuickFlux
  PREFIX "${PROJECT_BINARY_DIR}/QuickFlux-build"
  SOURCE_DIR "${PROJECT_SOURCE_DIR}/../.."
  CMAKE_ARGS "-DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}"
             "-DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/QuickFlux"
             "-DCMAKE_INSTALL_LIBDIR=${PROJECT_BINARY_DIR}/QuickFlux/lib"
             "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
  )

link_directories("${PROJECT_BINARY_DIR}/QuickFlux/lib")
include_directories("${PROJECT_BINARY_DIR}/QuickFlux/include/quickflux")

add_executable(todo WIN32 ${todo_SRCS})
add_dependencies(todo QuickFlux)

target_link_libraries(todo debug quickfluxd optimized quickflux)
target_link_libraries(todo Qt5::Qml Qt5::Gui Qt5::Core Qt5::Quick)
