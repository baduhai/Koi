cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)

project(Koi)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS ui)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#ECM is extra cmake modules and is used by kde frameworks
find_package(ECM ${KF5_MIN_VER} CONFIG REQUIRED )

set(QT_MIN_VERSION "5.14.0")
set(KF5_MIN_VERSION "5.38.0")
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMOptionalAddSubdirectory)
include(ECMAddAppIcon)
include(ECMInstallIcons)

find_package(Qt5 ${REQUIRED_QT_VERSION} COMPONENTS NO_MODULE REQUIRED Network DBus Gui Core Test)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS CoreAddons WidgetsAddons Config  Package )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/headers )

add_subdirectory(src)