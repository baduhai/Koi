
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#QT and kde libraries 
set(QT5_LIBRARIES Qt5::Core Qt5::Gui Qt5::DBus Qt5::Network Qt5::Test)
set(KF5_LIBRAIRES KF5::CoreAddons KF5::WidgetsAddons KF5::ConfigCore  KF5::ConfigGui KF5::Package KF5::I18n KF5::Service)
set(koi_SRCS
    about.cpp 
    colorscheme.cpp 
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


add_executable(koi ${koi_SRCS})
target_link_libraries(koi ${QT5_LIBRARIES} ${KF5_LIBRAIRES})
    