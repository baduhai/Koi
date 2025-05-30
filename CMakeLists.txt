cmake_minimum_required(VERSION 3.16)
project(Koi VERSION 0.5.1 LANGUAGES CXX)

configure_file(src/headers/config.h.in config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Define Installation Data-Root Directory
set(CMAKE_INSTALL_DATAROOTDIR   share)
# Define *SYSTEM-WIDE* DBUS_INTERFACES_INSTALL_DIR variable
set(DBUS_INTERFACES_INSTALL_DIR share/dbus-1/interfaces)
# Define *SYSTEM-WIDE* DBUS_SERVICES_INSTALL_DIR variable
set(DBUS_SERVICES_INSTALL_DIR share/dbus-1/services)

# Configure DBus Service File
configure_file(src/resources/dbus/services/dev.baduhai.Koi.service ${CMAKE_CURRENT_BINARY_DIR}/dev.baduhai.Koi.service)

# Install files not being a part of INSTALL TARGETS section
install(FILES  ${CMAKE_CURRENT_BINARY_DIR}/dev.baduhai.Koi.xml     DESTINATION ${DBUS_INTERFACES_INSTALL_DIR})
install(FILES  src/koi.desktop                                     DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications
        RENAME local.KoiDbusInterface.desktop)
install(FILES  src/resources/dbus/services/dev.baduhai.Koi.service DESTINATION ${DBUS_SERVICES_INSTALL_DIR})
install(FILES  src/resources/icons/koi.svg                         DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps)
install(FILES  src/resources/icons/koi_tray.svg                    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps)

# Look for DEPENDENCIES
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(KF6Config)
find_package(KF6CoreAddons)
find_package(KF6WidgetsAddons)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS DBus Gui)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Xml)

# Set SOURCES into a single BINARY
set(koi_SRC
    # Main app stuff.
    src/about.cpp
    src/license.cpp
    src/trayManager.cpp
    src/mainwindow.cpp
    src/utils.cpp
    src/runguard.cpp
    # QObject headers needs to be added as sources to properly run MOC and UIC.
    src/headers/about.h
    src/headers/license.h
    src/headers/trayManager.h
    src/headers/mainwindow.h
    src/headers/utils.h
    src/headers/runguard.h
    # DBus interface stuff
    src/dbusinterface.cpp
    src/headers/dbusinterface.h
    # DBus interface XML file -- generated at build time
    ${CMAKE_CURRENT_BINARY_DIR}/dev.baduhai.Koi.xml
    # DBus interface adapter -- generated at build time from the XML file
    ${CMAKE_CURRENT_BINARY_DIR}/koiadaptor.h
    # UI items
    src/resources/resources.qrc
    src/ui/about.ui
    src/ui/license.ui
    src/ui/mainwindow.ui
    # Libraries for additional stuff
    src/libraries/SunRise.cpp
    # All plugins
    src/plugins/colorscheme.cpp
    src/plugins/gtk.cpp
    src/plugins/icons.cpp
    src/plugins/kvantumstyle.cpp
    src/plugins/plasmastyle.cpp
    src/plugins/wallpaper.cpp
    src/plugins/script.cpp
    # Main entrypoint
    src/main.cpp
)

qt_generate_dbus_interface(src/headers/dbusinterface.h
    dev.baduhai.Koi.xml
    OPTIONS -A
)

qt_add_dbus_adaptor(koi_SRC ${CMAKE_CURRENT_BINARY_DIR}/dev.baduhai.Koi.xml
                     src/headers/dbusinterface.h KoiDbusInterface)


add_executable(koi
    ${koi_SRC}
)

target_compile_definitions(koi PUBLIC
    QT_DEPRECATED_WARNINGS
)

# Link SYSTEM libraries with the Koi project
target_link_libraries(koi PUBLIC
    KF6::ConfigCore
    KF6::ConfigGui
    KF6::CoreAddons
    KF6::WidgetsAddons
    Qt::Core
    Qt::DBus
    Qt::Gui
    Qt::Widgets
    Qt::Xml
    Threads::Threads
)

# Install target
install(TARGETS koi
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)

# Uninstall target
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  @ONLY
)
add_custom_target(uninstall
  COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
)
