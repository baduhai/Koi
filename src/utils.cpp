#include "headers/utils.h"

Utils::Utils()
{
}

// Global settings stuff
void Utils::initialiseSettings()
{
    //settings = new QSettings("koirc", QSettings::IniFormat); // Line used for testing !Must comment before pushing!
    settings = new QSettings(QDir::homePath() + "/.config/koirc", QSettings::IniFormat); // Setting config path and format
}

// Miscelaneous functions
void Utils::notify(QString notifySummary, QString notifyBody, int timeoutms) // Push notification through DBus
{
    bus = new QDBusConnection(QDBusConnection::sessionBus());
    notifyInterface = new QDBusInterface("org.freedesktop.Notifications", "/org/freedesktop/Notifications", "org.freedesktop.Notifications", *bus);
    QString app_name = "Koi";        // What program is the notification coming from?
    uint replaces_id = 0;            // Not sure what this is. Think it has something to do with pid.
    QString app_icon;                // Not actually specifying app icon, this is if you'd like to push an image alog with notification.
    QString summary = notifySummary; // Title of notification.
    QString body = notifyBody;       // Notification body.
    QStringList actions;             // No idea how to use.
    QVariantMap hints;               // No idea how to use.
    int timeout = timeoutms;         // Notification timeout, there's no way to assume system has a default timeout unfortunately.
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
    colorsLocalDir.setNameFilters(QStringList() << "*.colors");
    colorsLocalDir.setFilter(QDir::Files);
    colorsLocalDir.setSorting(QDir::Name);
    QList<QFileInfo> colorSchemesLocal = colorsLocalDir.entryInfoList();
    QStringList colorSchemesLocalNames;
    for (int i = 0; i < colorSchemesLocal.size(); i++)
    {
        colorSchemesLocalNames.append(colorSchemesLocal.at(i).baseName());
    }
    QDir colorsSystemDir("/usr/share/color-schemes");
    colorsSystemDir.setNameFilters(QStringList() << "*.colors");
    colorsSystemDir.setFilter(QDir::Files);
    colorsSystemDir.setSorting(QDir::Name);
    QList<QFileInfo> colorSchemesSystem = colorsSystemDir.entryInfoList();
    QStringList colorSchemesSystemNames;
    for (int i = 0; i < colorSchemesSystem.size(); i++)
    {
        colorSchemesSystemNames.append(colorSchemesSystem.at(i).baseName());
    }
    QStringList colorSchemesNames = colorSchemesSystemNames + colorSchemesLocalNames;
    return colorSchemesNames;
}
QStringList Utils::getColorSchemesPath(void) // Get all available color schemes
{
    QDir colorsLocalDir(QDir::homePath() + "/.local/share/color-schemes");
    colorsLocalDir.setNameFilters(QStringList() << "*.colors");
    colorsLocalDir.setFilter(QDir::Files);
    colorsLocalDir.setSorting(QDir::Name);
    QList<QFileInfo> colorSchemesLocal = colorsLocalDir.entryInfoList();
    QStringList colorSchemesLocalPath;
    for (int i = 0; i < colorSchemesLocal.size(); i++)
    {
        colorSchemesLocalPath.append(colorSchemesLocal.at(i).absoluteFilePath());
    }
    QDir colorsSystemDir("/usr/share/color-schemes");
    colorsSystemDir.setNameFilters(QStringList() << "*.colors");
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
QStringList Utils::getKvantumStyles(void) // Get all available kvantum styles
{
    QDir kvantumStyleLocalDir(QDir::homePath() + "/.config/Kvantum");
    QDir kvantumStyleSystemDir("/usr/share/Kvantum");
    QStringList kvantumStyles;
    if(kvantumStyleLocalDir.exists()){
        kvantumStyles.append(kvantumStyleLocalDir.entryList(QDir::Dirs));
    }
    if(kvantumStyleSystemDir.exists()){
        kvantumStyles.append(kvantumStyleSystemDir.entryList(QDir::Dirs));
    }
    kvantumStyles.removeDuplicates();
    kvantumStyles.removeFirst();
    kvantumStyles.removeFirst();
    return kvantumStyles;
}
// Manage switching themes functions
void Utils::goLight()
{
    goLightStyle();
    goLightColors();
    goLightIcons();
    goLightGtk();
    goLightKvantumStyle();
    goLightWall();
    restartProcess();
    if (settings->value("notify").toBool())
    {
        notify("Switched to light mode!", "Some applications may need to be restarted for applied changes to take effect.");
    }
}
void Utils::goDark()
{
    goDarkStyle();
    goDarkColors();
    goDarkIcons();
    goDarkGtk();
    goDarkKvantumStyle();
    goDarkWall();
    restartProcess();
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
void Utils::goLightKvantumStyle()
{
    if (settings->value("KvantumStyle/enabled").toBool())
    {
        kvantumStyle.setKvantumStyle(settings->value("KvantumStyle/light").toString());
    }
}
void Utils::goDarkKvantumStyle()
{
    if (settings->value("KvantumStyle/enabled").toBool())
    {
        kvantumStyle.setKvantumStyle(settings->value("KvantumStyle/dark").toString());
    }
}
void Utils::goLightWall()
{
    if (settings->value("Wallpaper/enabled").toBool())
    {
        if (!settings->value("Wallpaper/light").isNull())
        {
            wallpaper.setWallpaper(settings->value("Wallpaper/light").toString());
        }
        else
        {
            notify("Error setting Wallpaper", "Koi tried to change your wallpaper, but no wallpaper fie was selected", 0);
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
        else
        {
            notify("Error setting Wallpaper", "Koi tried to change your wallpaper, but no wallpaper file was selected", 0);
        }
    }
}
/* this updates the style of both the plasma shell and latte dock if it is available 
*/
void Utils::restartProcess()
{
    if (settings->value("KvantumStyle/enabled").toBool())
    {
        killAllProcess = new QProcess;
        QString killAll = "/usr/bin/killall"; //used to kill a process

        kstart5Process = new QProcess;
        QString kstart5 = "/usr/bin/kstart5";
        QStringList plasmashell = {"plasmashell"};

        killAllProcess->start(killAll, plasmashell);
        kstart5Process->start(kstart5, plasmashell);

    }
}
