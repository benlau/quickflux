#
# To build it with cmake, you should register qml types explicitly by calling registerQuickFluxQmlTypes() in your main.cpp
# See examples/middleware for example
#

cmake_minimum_required(VERSION 3.0.0)
project(quickflux VERSION 1.1.3)

option(quickflux_INSTALL "Enable the installation of targets." ON)

if(MSVC)
  set_property (GLOBAL PROPERTY USE_FOLDERS ON)
endif()

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

include(GNUInstallDirs)
find_package(Qt5 COMPONENTS Core Quick Qml Gui CONFIG REQUIRED)

set(quickflux_PRIVATE_SOURCES
  ${SRC_DIR}/priv/qfhook.cpp
  ${SRC_DIR}/priv/qfmiddlewareshook.cpp
  ${SRC_DIR}/priv/qfsignalproxy.cpp
  ${SRC_DIR}/priv/quickfluxfunctions.cpp
  )

set(quickflux_PUBLIC_SOURCES
  ${SRC_DIR}/qfactioncreator.cpp
  ${SRC_DIR}/qfappdispatcher.cpp
  ${SRC_DIR}/qfapplistener.cpp
  ${SRC_DIR}/qfapplistenergroup.cpp
  ${SRC_DIR}/qfappscript.cpp
  ${SRC_DIR}/qfappscriptdispatcherwrapper.cpp
  ${SRC_DIR}/qfappscriptgroup.cpp
  ${SRC_DIR}/qfappscriptrunnable.cpp
  ${SRC_DIR}/qfdispatcher.cpp
  ${SRC_DIR}/qffilter.cpp
  ${SRC_DIR}/qfhydrate.cpp
  ${SRC_DIR}/qfkeytable.cpp
  ${SRC_DIR}/qflistener.cpp
  ${SRC_DIR}/qfmiddleware.cpp
  ${SRC_DIR}/qfmiddlewarelist.cpp
  ${SRC_DIR}/qfobject.cpp
  ${SRC_DIR}/qfqmltypes.cpp
  ${SRC_DIR}/qfstore.cpp
  )

set(quickflux_PRIVATE_HEADERS
  ${SRC_DIR}/priv/qfappscriptdispatcherwrapper.h
  ${SRC_DIR}/priv/qfappscriptrunnable.h
  ${SRC_DIR}/priv/qfhook.h
  ${SRC_DIR}/priv/qflistener.h
  ${SRC_DIR}/priv/qfmiddlewareshook.h
  ${SRC_DIR}/priv/qfsignalproxy.h
  ${SRC_DIR}/priv/quickfluxfunctions.h
  )

set(quickflux_PUBLIC_HEADERS
  ${SRC_DIR}/qfactioncreator.h
  ${SRC_DIR}/QFAppDispatcher
  ${SRC_DIR}/qfapplistener.h
  ${SRC_DIR}/qfapplistenergroup.h
  ${SRC_DIR}/qfappscript.h
  ${SRC_DIR}/qfappdispatcher.h
  ${SRC_DIR}/qfappscriptgroup.h
  ${SRC_DIR}/qfdispatcher.h
  ${SRC_DIR}/qffilter.h
  ${SRC_DIR}/qfhydrate.h
  ${SRC_DIR}/QFKeyTable
  ${SRC_DIR}/qfkeytable.h
  ${SRC_DIR}/qfmiddleware.h
  ${SRC_DIR}/qfmiddlewarelist.h
  ${SRC_DIR}/qfobject.h
  ${SRC_DIR}/qfstore.h
  ${SRC_DIR}/QuickFlux
  )

source_group(include FILES
  ${quickflux_PRIVATE_HEADERS}
  ${quickflux_PUBLIC_HEADERS}
  )

if(MSVC)
  source_group("Source Files" FILES ${quickflux_PUBLIC_SOURCES})
  source_group("Source Files\\Private" FILES ${quickflux_PRIVATE_SOURCES})
  source_group("Header Files" FILES ${quickflux_PUBLIC_HEADERS})
  source_group("Header Files\\Private" FILES ${quickflux_PRIVATE_HEADERS})
  source_group("Source Files\\MOC" REGULAR_EXPRESSION "moc*")
endif()

add_library(quickflux STATIC
  ${quickflux_PRIVATE_SOURCES}
  ${quickflux_PRIVATE_HEADERS}
  ${quickflux_PUBLIC_SOURCES}
  ${quickflux_PUBLIC_HEADERS}
  ${moc}
  )
add_library(QuickFlux::quickflux ALIAS quickflux)

target_link_libraries(quickflux
  PUBLIC
  Qt5::Qml
  Qt5::Quick
  Qt5::Core
  )

target_include_directories(quickflux
  PUBLIC
  "$<BUILD_INTERFACE:${SRC_DIR}>"
  "$<INSTALL_INTERFACE:include/quickflux>"
  )

set_target_properties(quickflux PROPERTIES
  AUTOMOC TRUE
  DEBUG_POSTFIX d
  )

if(quickflux_INSTALL)

  install(TARGETS quickflux EXPORT QuickFluxTargets
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/quickflux"
    )

  install(FILES
    ${quickflux_PUBLIC_HEADERS}
    DESTINATION include/quickflux
    )
  install(FILES
    ${quickflux_PRIVATE_HEADERS}
    DESTINATION include/quickflux/priv
    )


  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
      ${CMAKE_BINARY_DIR}/cmake/QuickFluxVersion.cmake
      VERSION ${PROJECT_VERSION}
      COMPATIBILITY SameMajorVersion
  )

  # installation - build tree specific package config files
  export(EXPORT QuickFluxTargets FILE ${CMAKE_BINARY_DIR}/QuickFluxTargets.cmake)
  configure_file(${PROJECT_SOURCE_DIR}/QuickFluxConfig.cmake.in
      ${CMAKE_BINARY_DIR}/QuickFluxConfig.cmake
      COPYONLY
  )

  # installation - relocatable package config files
  configure_package_config_file(${PROJECT_SOURCE_DIR}/QuickFluxConfig.cmake.in
                                ${CMAKE_CURRENT_BINARY_DIR}/cmake/QuickFluxConfig.cmake
                                INSTALL_DESTINATION cmake
  )

  set(CONFIG_PACKAGE_LOCATION ${CMAKE_INSTALL_LIBDIR}/cmake/QuickFlux)

  install(EXPORT QuickFluxTargets
      FILE QuickFluxTargets.cmake
      NAMESPACE QuickFlux::
      DESTINATION ${CONFIG_PACKAGE_LOCATION}
  )

  install(FILES
      ${CMAKE_BINARY_DIR}/cmake/QuickFluxConfig.cmake
      ${CMAKE_BINARY_DIR}/cmake/QuickFluxVersion.cmake
      DESTINATION ${CONFIG_PACKAGE_LOCATION}
  )

endif()
