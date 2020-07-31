cmake_minimum_required(VERSION 3.17.0 FATAL_ERROR)

project(koi LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS ui)
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()
#ECM is extra cmake modules and is used by kde frameworks
find_package(ECM ${KF5_MIN_VER} CONFIG REQUIRED )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
set(QT_MIN_VERSION "5.14.0")
set(KF5_MIN_VERSION "5.38.0")
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake ${ECM_KDE_MODULE_DIR})
set(QT5_LIBRARIES Qt5::Core Qt5::Gui Qt5::DBus Qt5::Network Qt5::Test)
set(KF5_LIBRAIRES KF5::CoreAddons KF5::WidgetsAddons KF5::ConfigCore  KF5::ConfigGui KF5::Package KF5::I18n KF5::Service)
set(koi_SRCS
 colorscheme.cpp 
    about.cpp 
    gtk.cpp 
    icons.cpp 
    license.cpp 
    main.cpp 
    mainwindow.cpp 
    plasmastyle.cpp 
    utils.cpp 
    wallpaper.cpp 
    kvantumstyle.cpp 
   # lnflistmodel.cpp 
   # lnflogic.cpp
)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMOptionalAddSubdirectory)
include(ECMAddAppIcon)
include(ECMInstallIcons)
find_package(Threads)

find_package(Qt5 ${REQUIRED_QT_VERSION} COMPONENTS NO_MODULE REQUIRED Network DBus Gui Core Test)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS CoreAddons WidgetsAddons Config  Package I18n Service)


add_executable(${PROJECT_NAME} ${koi_SRCS} )
target_link_libraries(${PROJECT_NAME} )
target_link_libraries(${PROJECT_NAME} ${QT5_LIBRARIES} ${KF5_LIBRAIRES} Threads::Threads)