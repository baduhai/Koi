# Generated from Koi.pro.

cmake_minimum_required(VERSION 3.16)
project(Koi VERSION 0.1.5 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "/usr/bin/")

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS DBus Gui Network Test)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)
find_package(KF5Config)
find_package(KF5CoreAddons)
find_package(KF5WidgetsAddons)

add_executable(Koi
    libs/Cron.h
    libs/InterruptableSleep.h
    libs/Scheduler.h
    libs/SunRise.cpp libs/SunRise.h
    
    resources/resources.qrc
    
    ui/about.ui
    ui/license.ui
    ui/mainwindow.ui
    
    about.cpp headers/about.h
    colorscheme.cpp headers/colorscheme.h
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

target_compile_definitions(Koi PUBLIC
    QT_DEPRECATED_WARNINGS
)

target_link_libraries(Koi PUBLIC
    KF5::ConfigCore
    KF5::ConfigGui
    KF5::CoreAddons
    KF5::WidgetsAddons
    Qt::Core
    Qt::DBus
    Qt::Gui
    Qt::Network
    Qt::Test
    Qt::Widgets
)

#if((QT_VERSION_MAJOR GREATER 4))
    #target_link_libraries(Koi PUBLIC
        #Qt::Widgets
    #)
#endif()

install(TARGETS Koi
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
