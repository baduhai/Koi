cmake_minimum_required(VERSION 3.16)
project(Koi VERSION 0.6 LANGUAGES CXX)

include(GNUInstallDirs)

# Configure and Include configuration files
configure_file(src/headers/config.h.in config.h)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
#
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
# Pre-set environmental variables for CMake BINARY
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
#
# Define Installation Data-Root Directory
set(CMAKE_INSTALL_DATAROOTDIR   share)
#

# Define *SYSTEM-WIDE* DBUS_INTERFACES_INSTALL_DIR variable
set(DBUS_INTERFACES_INSTALL_DIR share/dbus-1/interfaces)
#
# Define *SYSTEM-WIDE* DBUS_SERVICES_INSTALL_DIR variable
set(DBUS_SERVICES_INSTALL_DIR share/dbus-1/services)
#

# Define environmental variables for Guix GNU/Linux ->
# Patch Nix-specific paths in source files to Guix Linux equivalents; Might require manual adjustments according to one's needs . . .
# For `colorscheme.cpp`
file(READ "src/plugins/colorscheme.cpp" COLORS_CONTENT)
string(REPLACE "/var/run/current-system/sw/share/color-schemes" "/run/current-system/profile/share/color-schemes" COLORS_CONTENT "${COLORS_CONTENT}")
file(WRITE "src/plugins/colorscheme.cpp" "${COLORS_CONTENT}")
#
# For `kvantumstyle.cpp`
file(READ "src/plugins/kvantumstyle.cpp" KVANTUM_CONTENT)
string(REPLACE "/var/run/current-system/sw/Kvantum" "/run/current-system/profile/share/Kvantum" KVANTUM_CONTENT "${KVANTUM_CONTENT}")
file(WRITE "src/plugins/kvantumstyle.cpp" "${KVANTUM_CONTENT}")
#
# For `icons.cpp`
file(READ "src/plugins/icons.cpp" ICONS_CONTENT)
string(REPLACE "/var/run/current-system/sw/share/icons" "/run/current-system/profile/share/icons" ICONS_CONTENT "${ICONS_CONTENT}")
file(WRITE "src/plugins/icons.cpp" "${ICONS_CONTENT}")
#
# For `plasmastyle.cpp`
file(READ "src/plugins/plasmastyle.cpp" PLASMA_CONTENT)
string(REPLACE "/var/run/current-system/sw/share/plasma/desktoptheme" "/run/current-system/profile/share/plasma/desktoptheme" PLASMA_CONTENT "${PLASMA_CONTENT}")
file(WRITE "src/plugins/plasmastyle.cpp" "${PLASMA_CONTENT}")
#
# For `gtk.cpp`
file(READ "src/plugins/gtk.cpp" GTK_CONTENT)
string(REPLACE "/var/run/current-system/sw/share/themes" "/run/current-system/profile/share/themes" GTK_CONTENT "${GTK_CONTENT}")
file(WRITE "src/plugins/gtk.cpp" "${GTK_CONTENT}")
#
#

# Install files not being a part of INSTALL TARGETS section:
# --- Configure `.desktop` and D-BUS service with installed absolute paths ---
# Read and patch the `.desktop` file so `Exec=` uses the install prefix+bindir
file(READ  "${CMAKE_CURRENT_SOURCE_DIR}/src/koi.desktop"  DESKTOP_CONTENT)
string(REPLACE "Exec=koi"     "Exec=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/koi"     DESKTOP_CONTENT  "${DESKTOP_CONTENT}")
string(REPLACE "TryExec=koi"  "TryExec=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/koi"  DESKTOP_CONTENT  "${DESKTOP_CONTENT}")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/koi.desktop"  "${DESKTOP_CONTENT}")
#
# Read and patch the D-Bus service file so Exec uses the install prefix+bindir
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/src/resources/dbus/services/dev.baduhai.Koi.service"  DBUS_SERVICE_CONTENT)
string(REPLACE "Exec=koi"     "Exec=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/koi"     DBUS_SERVICE_CONTENT  "${DBUS_SERVICE_CONTENT}")
string(REPLACE "TryExec=koi"  "TryExec=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/koi"  DBUS_SERVICE_CONTENT  "${DBUS_SERVICE_CONTENT}")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/dev.baduhai.Koi.service"  "${DBUS_SERVICE_CONTENT}")
#
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/dev.baduhai.Koi.xml"     DESTINATION "${DBUS_INTERFACES_INSTALL_DIR}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/koi.desktop"             DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications"
        RENAME local.KoiDbusInterface.desktop)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/dev.baduhai.Koi.service" DESTINATION "${DBUS_SERVICES_INSTALL_DIR}")
install(FILES  src/resources/icons/koi.svg                          DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps")
install(FILES  src/resources/icons/koi_tray.svg                     DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps")
#
#

# Look for DEPENDENCIES
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(KF6Config)
find_package(KF6CoreAddons)
find_package(KF6WidgetsAddons)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package("Qt${QT_VERSION_MAJOR}" REQUIRED COMPONENTS DBus Gui)
find_package("Qt${QT_VERSION_MAJOR}" OPTIONAL_COMPONENTS Widgets)
find_package("Qt${QT_VERSION_MAJOR}" OPTIONAL_COMPONENTS Xml)
#
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
    "${CMAKE_CURRENT_BINARY_DIR}/dev.baduhai.Koi.xml"
    # DBus interface adapter -- generated at build time from the XML file
    "${CMAKE_CURRENT_BINARY_DIR}/koiadaptor.h"
    # UI items
    src/resources/resources.qrc
    src/ui/about.ui
    src/ui/license.ui
    src/ui/mainwindow.ui
    # Libraries for additional stuff
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
#
# Setup and Configure DBus Interface
qt_generate_dbus_interface("src/headers/dbusinterface.h"  "dev.baduhai.Koi.xml"  OPTIONS -A)

qt_add_dbus_adaptor(koi_SRC  "${CMAKE_CURRENT_BINARY_DIR}/dev.baduhai.Koi.xml"  "src/headers/dbusinterface.h"  KoiDbusInterface)
#
# Generate Executable BINARY
add_executable(koi  "${koi_SRC}")
#
# Ensure deprecated compiler warnings
target_compile_definitions(koi  PUBLIC  QT_DEPRECATED_WARNINGS)
#

# Link SYSTEM libraries with the Koi project
target_link_libraries(koi  PUBLIC
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
#
# Installation target
install(TARGETS koi
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION  "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
#
# Uninstallation target
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  @ONLY
)
add_custom_target(uninstall
  COMMAND  "${CMAKE_COMMAND}" -P  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
)
#
