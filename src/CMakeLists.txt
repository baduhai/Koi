#i currently cannot nest cmakelists because it breaks don't know why don't know how.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#The list of all the source files
set(koi_SRC
        about.cpp
        gtk.cpp
        license.cpp
        main.cpp
        mainwindow.cpp
        utils.cpp
        wallpaper.cpp
        kvantumstyle.cpp
        #Settings Dir
        settings/settingdialog.cpp
        settings/generalsettings.cpp
        settings/profilesettingsdialog.cpp
        settings/scheduleprofile.cpp
        #Settings/Profile
        settings/profiles/editprofiledialog.cpp
        settings/profiles/profile.cpp
        settings/profiles/profilemanager.cpp
        )

set(koi_RCS
        ../resources/resources.qrc )

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

