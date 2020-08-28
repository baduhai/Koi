set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#The list of all the source files
set(koi_SRC
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
        profile.cpp profile.h)

set(koi_RCS
        ../resources/resources.qrc)

set(icons
        ../resources/icons/koi.svg
        ../resources/icons/koi_tray.svg)

find_package(Threads)


add_executable(${PROJECT_NAME} ${koi_SRC}  ${koi_RCS})
target_link_libraries(${PROJECT_NAME} ${QT5_LIBRARIES} ${KF5_LIBRARIES} Threads::Threads)

# Install the executable
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(PROGRAMS ${PROJECT_SOURCE_DIR}/koi.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES ${icons} DESTINATION /usr/share/icons/hicolor/scalable/apps/)

