#include "headers/utils.h"

Utils::Utils()
{

}

// Light/Dark management


// Get stuff
QStringList Utils::getPlasmaStyles(void) // Get all available plasma styles
{
    QDir stylesLocalDir(QDir::homePath() + "/.local/share/plasma/desktoptheme");
    QDir stylesSystemDir("/usr/share/plasma/desktoptheme");
    QStringList plasmaStyles = stylesLocalDir.entryList(QDir::Dirs) + stylesSystemDir.entryList(QDir::Dirs);
    plasmaStyles.removeDuplicates();
    plasmaStyles.removeFirst();
    plasmaStyles.removeFirst();
    return plasmaStyles;
}
QStringList Utils::getColorSchemes(void) // Get all available color schemes
{
    QDir colorsLocalDir(QDir::homePath() + "/.local/share/color-schemes");
    QDir colorsSystemDir("/usr/share/color-schemes");
    colorsLocalDir.setNameFilters(QStringList()<<"*.colors");
    colorsSystemDir.setNameFilters(QStringList()<<"*.colors");
    QStringList colorSchemes = colorsLocalDir.entryList() + colorsSystemDir.entryList();
    colorSchemes.removeDuplicates();
    colorSchemes.removeFirst();
    colorSchemes.removeFirst();
    return colorSchemes;
}
QStringList Utils::getIconThemes(void) // Get all available icont themes
{
    QDir iconsOldLocalDir(QDir::homePath() + "/.icons");
    QDir iconsLocalDir(QDir::homePath() + "/.local/share/icons");
    QDir iconsSystemDir("/usr/share/icons");
    QStringList iconThemes = iconsOldLocalDir.entryList(QDir::Dirs) + iconsLocalDir.entryList(QDir::Dirs) + iconsSystemDir.entryList(QDir::Dirs);
    iconThemes.removeDuplicates();
    iconThemes.removeFirst();
    iconThemes.removeFirst();
    return iconThemes;
}
QStringList Utils::getGtkThemes(void) // Get all available gtk themes
{
    QDir gtkLocalDir(QDir::homePath() + "/.themes");
    QDir gtkSystemDir("/usr/share/themes");
    QStringList gtkThemes = gtkLocalDir.entryList(QDir::Dirs) + gtkSystemDir.entryList(QDir::Dirs);
    gtkThemes.removeDuplicates();
    gtkThemes.removeFirst();
    gtkThemes.removeFirst();
    return gtkThemes;
}

// Manage switching themes functions
void Utils::timeLoopLight()
{

}
void Utils::goLight()
{
    goLightGtk();
    goLightWall();
}
void Utils::goDark()
{
    goDarkGtk();
    goDarkWall();
}
void Utils::goLightGtk()
{
    if (settings->value("GTKTheme/enabled").toBool())
    {
        gtk.setGtk(settings->value("GTKTheme/light").toString());
    }
}
void Utils::goDarkGtk()
{
    if (settings->value("GTKTheme/enabled").toBool())
    {
        gtk.setGtk(settings->value("GTKTheme/dark").toString());
    }
}
void Utils::goLightWall()
{
    if (settings->value("Wallpaper/enabled").toBool())
    {
        if (!settings->value("Wallpaper/light").isNull()){
            wallpaper.setWallpaper(settings->value("Wallpaper/light").toString());
        }
    }
}

void Utils::goDarkWall()
{
    if (settings->value("Wallpaper/enabled").toBool())
    {
        if (!settings->value("Wallpaper/dark").isNull())
        {
            wallpaper.setWallpaper(settings->value("Wallpaper/dark").toString());
        }
    }
}
