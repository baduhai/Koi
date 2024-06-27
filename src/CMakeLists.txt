cmake_minimum_required(VERSION 3.16)
project(Koi VERSION 0.3.1 LANGUAGES CXX)

configure_file(headers/config.h.in config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/koi.desktop DESTINATION share/applications)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/koi.svg DESTINATION share/icons/hicolor/scalable/apps)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/koi_tray.svg DESTINATION share/icons/hicolor/scalable/apps)

find_package(KF6Config)
find_package(KF6CoreAddons)
find_package(KF6WidgetsAddons)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS DBus Gui Network Test)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Xml)


add_executable(koi
    libraries/Cron.h
    libraries/InterruptableSleep.h
    libraries/Scheduler.h
    libraries/SunRise.cpp libraries/SunRise.h
    
    resources/resources.qrc
    
    ui/about.ui
    ui/license.ui
    ui/mainwindow.ui
    
    about.cpp headers/about.h
    colorscheme.cpp headers/colorscheme.h
    headers/config.h.in
    gtk.cpp headers/gtk.h
    icons.cpp headers/icons.h
    kvantumstyle.cpp headers/kvantumstyle.h
    license.cpp headers/license.h
    mainwindow.cpp headers/mainwindow.h
    plasmastyle.cpp headers/plasmastyle.h
    utils.cpp headers/utils.h
    wallpaper.cpp headers/wallpaper.h
    
    main.cpp
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
    Qt::Network
    Qt::Test
    Qt::Widgets
    Qt::Xml
)

install(TARGETS koi
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
