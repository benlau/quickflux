cmake_minimum_required(VERSION 3.1.0)

project(todo)

################# set KDE specific information #################
find_package(ECM REQUIRED NO_MODULE)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})


set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Qml QuickControls2)
find_package(KF5 REQUIRED COMPONENTS Kirigami2 I18n)

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
target_link_libraries(todo Qt5::Qml Qt5::Gui Qt5::Core Qt5::Quick KF5::I18n)
