cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)

project(koi LANGUAGES CXX)

#This is used for autogenerating headers, .ui, .qrc files in qt 
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#set(CMAKE_AUTOUIC_SEARCH_PATHS ui)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#ECM is a extra cmake module and is used by kde frameworks
find_package(ECM ${KF5_MIN_VER} CONFIG REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 ")
set(QT_MIN_VERSION "5.14.0")
set(KF5_MIN_VERSION "5.38.0")
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake ${ECM_KDE_MODULE_DIR})
set(QT5_LIBRARIES Qt5::Core Qt5::Gui Qt5::DBus Qt5::Network Qt5::Test Qt5::Widgets)
set(KF5_LIBRARIES KF5::CoreAddons KF5::WidgetsAddons KF5::ConfigCore KF5::ConfigGui KF5::Package KF5::Service)


include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMOptionalAddSubdirectory)
include(ECMAddAppIcon)
include(ECMInstallIcons)


add_definitions(-DQT_DEPRECATED_WARNINGS
        #QT_USE_STRINGBUILDER IS for reducing the footprint of concatenated strings in was used in utils.cpp
        -DQT_USE_QSTRINGBUILDER)

find_package(Qt5 ${REQUIRED_QT_VERSION} COMPONENTS NO_MODULE REQUIRED Network DBus Gui Core Test Widgets)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS CoreAddons WidgetsAddons Config Package Service)


add_subdirectory(src)

