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
        )

set(koi_RCS
        ../resources/resources.qrc)

find_package(Threads)


add_executable(${PROJECT_NAME} ${koi_SRC}  ${koi_RCS} )
target_link_libraries(${PROJECT_NAME} ${QT5_LIBRARIES} ${KF5_LIBRAIRES} Threads::Threads)
