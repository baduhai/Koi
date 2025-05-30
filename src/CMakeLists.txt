cmake_minimum_required(VERSION 3.16)
project(Koi VERSION 0.5 LANGUAGES CXX)

configure_file(headers/config.h.in config.h)
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
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources/dbus/services/dev.baduhai.Koi.service ${CMAKE_CURRENT_BINARY_DIR}/dev.baduhai.Koi.service)

# Install files not being a part of INSTALL TARGETS section
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/dev.baduhai.Koi.xml                             DESTINATION ${DBUS_INTERFACES_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/koi.desktop                                     DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications
        RENAME local.KoiDbusInterface.desktop)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/dbus/services/dev.baduhai.Koi.service DESTINATION ${DBUS_SERVICES_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/koi.svg                         DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/koi_tray.svg                    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(KF6Config)
find_package(KF6CoreAddons)
find_package(KF6WidgetsAddons)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS DBus Gui)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Xml)

set(koi_SRC
    # Main app stuff.
    about.cpp
    license.cpp
    mainwindow.cpp
    trayManager.cpp
    utils.cpp
    runguard.cpp
    # QObject headers needs to be added as sources to properly run MOC and UIC.
    headers/about.h
    headers/license.h
    headers/mainwindow.h
    headers/trayManager.h
    headers/utils.h
    headers/runguard.h
    # DBus interface stuff
    dbusinterface.cpp
    headers/dbusinterface.h
    # DBus interface XML file -- generated at build time
    ${CMAKE_CURRENT_BINARY_DIR}/dev.baduhai.Koi.xml
    # DBus interface adapter -- generated at build time from the XML file
    ${CMAKE_CURRENT_BINARY_DIR}/koiadaptor.h
    # UI items
    resources/resources.qrc
    ui/about.ui
    ui/license.ui
    ui/mainwindow.ui
    # Libraries for additional stuff
    libraries/SunRise.cpp
    # All plugins
    plugins/colorscheme.cpp
    plugins/gtk.cpp
    plugins/icons.cpp
    plugins/kvantumstyle.cpp
    plugins/plasmastyle.cpp
    plugins/wallpaper.cpp
    plugins/script.cpp
    # Main entrypoint
    main.cpp
)

qt_generate_dbus_interface(headers/dbusinterface.h
    dev.baduhai.Koi.xml
    OPTIONS -A
)

qt_add_dbus_adaptor(koi_SRC ${CMAKE_CURRENT_BINARY_DIR}/dev.baduhai.Koi.xml
                     headers/dbusinterface.h KoiDbusInterface)


add_executable(koi
    ${koi_SRC}
)

target_compile_definitions(koi PUBLIC
    QT_DEPRECATED_WARNINGS
)

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

install(TARGETS koi
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
