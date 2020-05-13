#include "headers/utils.h"

Utils::Utils()
{

}

// Global settings stuff
void Utils::initialiseSettings()
{
    settings = new QSettings("koirc", QSettings::IniFormat); // Line used for testing !Must comment before pushing!
    // settings = new QSettings(QDir::homePath() + "/.config/koirc", QSettings::IniFormat); // Setting config path and format
}

// Miscelaneous functions
void Utils::notify(QString notifySummary, QString notifyBody, int timeoutms = 5000) // Push notification through DBus
{
    bus = new QDBusConnection(QDBusConnection::sessionBus());
    notifyInterface = new QDBusInterface("org.freedesktop.Notifications", "/org/freedesktop/Notifications", "org.freedesktop.Notifications", *bus);
    QString app_name = "Koi"; // What program is the notification coming from?
    uint replaces_id = 0; // Not sure what this is. Think it has something to do with pid.
    QString app_icon; // Not actually specifying app icon, this is if you'd like to push an image alog with notification.
    QString summary = notifySummary; // Title of notification.
    QString body = notifyBody; // Notification body.
    QStringList actions; // No idea how to use.
    QVariantMap hints; // No idea how to use.
    int timeout = timeoutms; // Notification timeout, there's no way to assume system has a default timeout unfortunately.
    notifyInterface->call("Notify", app_name, replaces_id, app_icon, summary, body, actions, hints, timeout);
}
void Utils::startupTimeCheck() // Check if switching is needed based on time.
{
    QTime lightTime = QTime::fromString(settings->value("time-light").toString(), "hh:mm:ss");
    QTime darkTime = QTime::fromString(settings->value("time-dark").toString(), "hh:mm:ss");
    QTime now = QTime::currentTime();
    if (now < lightTime && now < darkTime)
    {
        QTest::qWait(1000); // Needed delay, or Koi may use the wrong color scheme.
        goDark();
    }
    else if (now == lightTime) // Highly unlikely
    {
        QTest::qWait(1000);
        goLight();
    }
    else if (now > lightTime && now < darkTime)
    {
        QTest::qWait(1000);
        goLight();
    }
    else if (now == darkTime) // Highly unlikely
    {
        QTest::qWait(1000);
        goDark();
    }
    else
    {
        QTest::qWait(1000);
        goDark();
    }
}
void Utils::timerToLight()
{
    // QTimer *lightTimer = new QTimer(this);
    lightTimer->setSingleShot(1);
    connect(lightTimer, &QTimer::timeout, this, &Utils::goLight);
    QTime setLightTime = QTime::fromString(settings->value("time-light").toString(), "hh:mm:ss");
    if (QTime::currentTime() >= setLightTime)
    {
        int toNextLight = (setLightTime.msecsTo(QTime::currentTime()) + setLightTime.msecsSinceStartOfDay());
        lightTimer->start(toNextLight);
    }
    else
    {
        int toNextLight = (QTime::currentTime().msecsTo(setLightTime));
        lightTimer->start(toNextLight);
    }
}
void Utils::timerToDark()
{
    // QTimer *darkTimer = new QTimer(this);
    darkTimer->setSingleShot(1);
    connect(darkTimer, &QTimer::timeout, this, &Utils::goDark);
    QTime setDarkTime = QTime::fromString(settings->value("time-dark").toString(), "hh:mm:ss");
    if (QTime::currentTime() >= setDarkTime)
    {
        int toNextDark = (setDarkTime.msecsTo(QTime::currentTime()) + setDarkTime.msecsSinceStartOfDay());
        darkTimer->start(toNextDark);
    }
    else
    {
        int toNextDark = (QTime::currentTime().msecsTo(setDarkTime));
        darkTimer->start(toNextDark);
    }
}

// Get stuff
QStringList Utils::getPlasmaStyles(void) // Get all available plasma styles
{
    QDir stylesLocalDir(QDir::homePath() + "/.local/share/plasma/desktoptheme");
    QDir stylesSystemDir("/usr/share/plasma/desktoptheme");
    QStringList plasmaStyles = stylesLocalDir.entryList(QDir::Dirs) + stylesSystemDir.entryList(QDir::Dirs);
    plasmaStyles.removeDuplicates();
    plasmaStyles.removeFirst();
    plasmaStyles.removeFirst();
    plasmaStyles.append("breeze");
    return plasmaStyles;
}
QStringList Utils::getColorSchemes(void) // Get all available color schemes
{
    QDir colorsLocalDir(QDir::homePath() + "/.local/share/color-schemes");
    colorsLocalDir.setNameFilters(QStringList()<<"*.colors");
    colorsLocalDir.setFilter(QDir::Files);
    colorsLocalDir.setSorting(QDir::Name);
    QList<QFileInfo> colorSchemesLocal = colorsLocalDir.entryInfoList();
    QStringList colorSchemesLocalNames;
    for (int i = 0; i < colorSchemesLocal.size(); i++)
    {
        colorSchemesLocalNames.append(colorSchemesLocal.at(i).baseName());
    }
    QDir colorsSystemDir("/usr/share/color-schemes");
    colorsSystemDir.setNameFilters(QStringList()<<"*.colors");
    colorsSystemDir.setFilter(QDir::Files);
    colorsSystemDir.setSorting(QDir::Name);
    QList<QFileInfo> colorSchemesSystem = colorsSystemDir.entryInfoList();
    QStringList colorSchemesSystemNames;
    for (int i = 0; i < colorSchemesSystem.size(); i++)
    {
        colorSchemesSystemNames.append(colorSchemesSystem.at(i).baseName());
    }
    QStringList colorSchemesNames = colorSchemesSystemNames+ colorSchemesLocalNames;
    return colorSchemesNames;
}
QStringList Utils::getColorSchemesPath(void) // Get all available color schemes
{
    QDir colorsLocalDir(QDir::homePath() + "/.local/share/color-schemes");
    colorsLocalDir.setNameFilters(QStringList()<<"*.colors");
    colorsLocalDir.setFilter(QDir::Files);
    colorsLocalDir.setSorting(QDir::Name);
    QList<QFileInfo> colorSchemesLocal = colorsLocalDir.entryInfoList();
    QStringList colorSchemesLocalPath;
    for (int i = 0; i < colorSchemesLocal.size(); i++)
    {
        colorSchemesLocalPath.append(colorSchemesLocal.at(i).absoluteFilePath());
    }
    QDir colorsSystemDir("/usr/share/color-schemes");
    colorsSystemDir.setNameFilters(QStringList()<<"*.colors");
    colorsSystemDir.setFilter(QDir::Files);
    colorsSystemDir.setSorting(QDir::Name);
    QList<QFileInfo> colorSchemesSystem = colorsSystemDir.entryInfoList();
    QStringList colorSchemesSystemPath;
    for (int i = 0; i < colorSchemesSystem.size(); i++)
    {
        colorSchemesSystemPath.append(colorSchemesSystem.at(i).absoluteFilePath());
    }
    QStringList colorSchemesPath = colorSchemesSystemPath + colorSchemesLocalPath;
    return colorSchemesPath;
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
void Utils::goLight()
{
    if (darkTimer->isActive())
    {
        darkTimer->stop();
    }
    //timerToDark();
    goLightStyle();
    goLightColors();
    goLightIcons();
    goLightGtk();
    goLightWall();
    if (settings->value("notify").toBool())
    {
        notify("Switched to light mode!", "Some applications may need to be restarted for applied changes to take effect.");
    }

}
void Utils::goDark()
{
    if (lightTimer->isActive())
    {
        lightTimer->stop();
    }
    darkTimer->stop();
    timerToLight();
    goDarkStyle();
    goDarkColors();
    goDarkIcons();
    goDarkGtk();
    goDarkWall();
    if (settings->value("notify").toBool())
    {
        notify("Switched to dark mode!", "Some applications may need to be restarted for applied changes to take effect.");
    }
}
void Utils::goLightStyle()
{
    if (settings->value("PlasmaStyle/enabled").toBool())
    {
        if (settings->value("PlasmaStyle/light") == "breeze") // Breeze style is set differently from others
        {
            plasmastyle.setPlasmaStyleBreeze();
        }
        else
        {
            plasmastyle.setPlasmaStyle(settings->value("PlasmaStyle/light").toString());
        }
    }
}
void Utils::goDarkStyle()
{
    if (settings->value("PlasmaStyle/enabled").toBool())
    {
        if (settings->value("PlasmaStyle/dark") == "breeze") // Breeze style is set differently from others
        {
            plasmastyle.setPlasmaStyleBreeze();
        }
        else
        {
            plasmastyle.setPlasmaStyle(settings->value("PlasmaStyle/dark").toString());
        }
    }
}
void Utils::goLightColors()
{
    if (settings->value("ColorScheme/enabled").toBool())
    {
        colorscheme.setColorScheme(settings->value("ColorScheme/light").toString());
    }
}
void Utils::goDarkColors()
{
    if (settings->value("ColorScheme/enabled").toBool())
    {
        colorscheme.setColorScheme(settings->value("ColorScheme/dark").toString());
    }
}
void Utils::goLightIcons()
{
    if (settings->value("IconTheme/enabled").toBool())
    {
        icons.setIcons(settings->value("IconTheme/light").toString());
    }
}
void Utils::goDarkIcons()
{
    if (settings->value("IconTheme/enabled").toBool())
    {
        icons.setIcons(settings->value("IconTheme/dark").toString());
    }
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
