QT       += core gui dbus testlib network KCoreAddons KWidgetsAddons KConfigCore 

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    about.cpp \
    colorscheme.cpp \
    gtk.cpp \
    icons.cpp \
    license.cpp \
    main.cpp \
    mainwindow.cpp \
    plasmastyle.cpp \
    utils.cpp \
    wallpaper.cpp \
    kvantumstyle.cpp \
    lnflistmodel.cpp \
    lnflogic.cpp
    

HEADERS += \
    headers/colorscheme.h \
    headers/gtk.h \
    headers/about.h \
    headers/icons.h \
    headers/license.h \
    headers/mainwindow.h \
    headers/plasmastyle.h \
    headers/utils.h \
    headers/wallpaper.h \ 
    headers/kvantumstyle.h \ 
    headers/lnflistmodel.h \
    headers/lnflogic.h \\
    libs/Cron.h \
    libs/InterruptableSleep.h \
    libs/Scheduler.h

FORMS += \
    ui/about.ui \
    ui/license.ui \
    ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/resources.qrc

# Managing what make install does
target.path = /usr/bin/
desktop.path = /usr/share/applications/
desktop.files += koi.desktop
icons.path = /usr/share/icons/hicolor/scalable/apps/
icons.files += resources/icons/koi.svg\
               resources/icons/koi_tray.svg
INSTALLS += target desktop icons
