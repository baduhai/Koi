cmake_minimum_required(VERSION 3.16)
project(Koi VERSION 0.7 LANGUAGES CXX)

include(GNUInstallDirs)

# Configure and include out-of-tree configuration header
configure_file(src/headers/config.h.in config.h)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Pre-set output directories inside the build tree
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Define system-wide D-Bus installation directories relative to CMAKE_INSTALL_PREFIX
set(DBUS_INTERFACES_INSTALL_DIR "${CMAKE_INSTALL_DATAROOTDIR}/dbus-1/interfaces"
    CACHE PATH "D-Bus interfaces installation directory")
set(DBUS_SERVICES_INSTALL_DIR "${CMAKE_INSTALL_DATAROOTDIR}/dbus-1/services"
    CACHE PATH "D-Bus services installation directory")

# Configure .desktop and D-Bus service files out-of-tree with resolved prefix/bindir
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/src/koi.desktop" DESKTOP_CONTENT)
string(REPLACE "Exec=koi"    "Exec=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/koi"    DESKTOP_CONTENT "${DESKTOP_CONTENT}")
string(REPLACE "TryExec=koi" "TryExec=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/koi" DESKTOP_CONTENT "${DESKTOP_CONTENT}")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/koi.desktop" "${DESKTOP_CONTENT}")

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/src/resources/dbus/services/dev.baduhai.Koi.service" DBUS_SERVICE_CONTENT)
string(REPLACE "Exec=koi"    "Exec=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/koi"    DBUS_SERVICE_CONTENT "${DBUS_SERVICE_CONTENT}")
string(REPLACE "TryExec=koi" "TryExec=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/koi" DBUS_SERVICE_CONTENT "${DBUS_SERVICE_CONTENT}")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/dev.baduhai.Koi.service" "${DBUS_SERVICE_CONTENT}")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/dev.baduhai.Koi.xml"     DESTINATION "${DBUS_INTERFACES_INSTALL_DIR}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/koi.desktop"             DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications"
        RENAME local.KoiDbusInterface.desktop)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/dev.baduhai.Koi.service" DESTINATION "${DBUS_SERVICES_INSTALL_DIR}")
install(FILES src/resources/icons/koi.svg                           DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps")
install(FILES src/resources/icons/koi_tray.svg                      DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps")

# Look for dependencies
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(KF6Config REQUIRED)
find_package(KF6CoreAddons REQUIRED)
find_package(KF6WidgetsAddons REQUIRED)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package("Qt${QT_VERSION_MAJOR}" REQUIRED COMPONENTS DBus Gui Widgets Xml)

# Set sources into a single binary
set(koi_SRC
    # Main app files
    src/about.cpp
    src/license.cpp
    src/trayManager.cpp
    src/mainwindow.cpp
    src/utils.cpp
    src/runguard.cpp
    # QObject headers needed for MOC and UIC
    src/headers/about.h
    src/headers/license.h
    src/headers/trayManager.h
    src/headers/mainwindow.h
    src/headers/utils.h
    src/headers/runguard.h
    # D-Bus interface files
    src/dbusinterface.cpp
    src/headers/dbusinterface.h
    # D-Bus interface XML and adaptor generated at build time
    "${CMAKE_CURRENT_BINARY_DIR}/dev.baduhai.Koi.xml"
    "${CMAKE_CURRENT_BINARY_DIR}/koiadaptor.h"
    # UI items
    src/resources/resources.qrc
    src/ui/about.ui
    src/ui/license.ui
    src/ui/mainwindow.ui
    # Additional libraries
    src/libraries/SunRise.cpp
    # All plugins
    src/plugins/aurorae.cpp
    src/plugins/colorscheme.cpp
    src/plugins/gtk.cpp
    src/plugins/icons.cpp
    src/plugins/konsole.cpp
    src/plugins/kvantumstyle.cpp
    src/plugins/plasmastyle.cpp
    src/plugins/wallpaper.cpp
    src/plugins/script.cpp
    # Main entrypoint
    src/main.cpp
)

# Setup and configure D-Bus interface
qt_generate_dbus_interface("src/headers/dbusinterface.h" "dev.baduhai.Koi.xml" OPTIONS -A)
qt_add_dbus_adaptor(koi_SRC "${CMAKE_CURRENT_BINARY_DIR}/dev.baduhai.Koi.xml" "src/headers/dbusinterface.h" KoiDbusInterface)

# Generate executable binary
add_executable(koi "${koi_SRC}")

# Enable deprecated Qt API warnings
target_compile_definitions(koi PUBLIC QT_DEPRECATED_WARNINGS)

# Link system libraries
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

# Installation target
install(TARGETS koi
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    BUNDLE  DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)

# Uninstallation target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    @ONLY
)
add_custom_target(uninstall
    COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
)
