
#include "utils.h"

Utils::Utils()
{
}

// Global settings stuff
void Utils::initialiseSettings()
{
    //fdfdfdf
    //settings = new QSettings("koirc", QSettings::IniFormat); // Line used for testing !Must comment before pushing!
    //TODO switch to koiglobalrc because a file is already using koirc (Koirc)
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
QStringList Utils::getPlasmaStyles() // Get all available plasma styles
{
    QDir stylesLocalDir(QDir::homePath() + "/.local/share/plasma/desktoptheme");
    QDir stylesSystemDir("/usr/share/plasma/desktoptheme");
    QStringList plasmaStyles;
    if (stylesLocalDir.exists())
    {
        plasmaStyles.append(stylesLocalDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    }
    if (stylesSystemDir.exists())
    {
        plasmaStyles.append(stylesSystemDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    }
    plasmaStyles.removeDuplicates();
    plasmaStyles.append("breeze");
    return plasmaStyles;
}
QStringList Utils::getColorSchemes() // Get all available color schemes
{
    QDir colorsLocalDir(QDir::homePath() + "/.local/share/color-schemes");
    colorsLocalDir.setNameFilters(QStringList() << "*.colors");
    colorsLocalDir.setFilter(QDir::Files);
    colorsLocalDir.setSorting(QDir::Name);
    QList<QFileInfo> colorSchemesLocal = colorsLocalDir.entryInfoList();
    QStringList colorSchemesLocalNames;
    //TODO remember to ask about qasconst in qt reddit group
    for (const auto &color : qAsConst(colorSchemesLocal))
    {
        colorSchemesLocalNames.append(color.baseName());
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
QStringList Utils::getColorSchemesPath() // Get all available color schemes
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
    for (const auto &colorSchemes : colorSchemesSystem)
    {
        colorSchemesSystemPath.append(colorSchemes.absoluteFilePath());
    }
    QStringList colorSchemesPath = colorSchemesSystemPath + colorSchemesLocalPath;
    return colorSchemesPath;
}
QStringList Utils::getIconThemes() // Get all available icon themes
{
    QDir iconsLocalDir(QDir::homePath() + "/.local/share/icons");
    QDir iconsSystemDir("/usr/share/icons");
    QStringList iconThemes = iconsLocalDir.entryList(QDir::Dirs) + iconsSystemDir.entryList(QDir::Dirs);
    iconThemes.removeDuplicates();
    iconThemes.removeFirst();
    iconThemes.removeFirst();
    return iconThemes;
}
QStringList Utils::getCursorThemes()
{
    QDir cursorOldLocalParentDir(QDir::homePath() + QStringLiteral("/.local/share/icons/"));
    QDir cursorLocalParentDir(QDir::homePath() + QStringLiteral("/.icons"));
    QDir cursorSystemParentDir(QStringLiteral("/usr/share/icons"));
    QFileInfoList parentDir(cursorOldLocalParentDir.entryInfoList(QDir::Dirs));
    parentDir.append(cursorSystemParentDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot));
    QStringList cursorThemes;
    //oldlocal and system
    for (QFileInfo &info : parentDir)
    {
        QDir filepath(info.absoluteFilePath() + QStringLiteral("/cursors"));
        if (filepath.exists())
        {
            cursorThemes.append(info.fileName());
        }
    }
    //local cursors
    cursorThemes.append(cursorLocalParentDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    return cursorThemes;
}

QStringList Utils::getGtkThemes() // Get all available gtk themes
{
    QDir gtkLocalDir(QDir::homePath() + "/.themes");
    QDir gtkSystemDir("/usr/share/themes");
    QStringList gtkThemes = gtkLocalDir.entryList(QDir::Dirs) + gtkSystemDir.entryList(QDir::Dirs);
    gtkThemes.removeDuplicates();
    gtkThemes.removeFirst();
    gtkThemes.removeFirst();
    return gtkThemes;
}
QStringList Utils::getKvantumStyles() // Get all available kvantum styles
{
    QDir kvantumStyleLocalDir(QDir::homePath() + "/.config/Kvantum");
    QDir kvantumStyleSystemDir("/usr/share/Kvantum");
    QStringList kvantumStyles = kvantumStyleLocalDir.entryList(QDir::Dirs) + kvantumStyleSystemDir.entryList(QDir::Dirs);
    kvantumStyles.removeDuplicates();
    kvantumStyles.removeFirst();
    kvantumStyles.removeFirst();
    return kvantumStyles;
}
QStringList Utils::getWidgetStyles()
{
    //this literally took me 2 hrs to find.
    QStringList widgetStyles = QStyleFactory::keys();
    return widgetStyles;
}

bool Utils::themeExists(QString themeName)
{
    QFileInfo localTheme(QDir::homePath() + QStringLiteral("/.local/share/plasma/look-and-feel/") + themeName + QStringLiteral("/contents/defaults"));
    return localTheme.exists() && localTheme.isFile();
}
void Utils::createNewTheme(const QString &pluginName, const QString &name, const QString &comment, const QString &author, const QString &email, const QString &license, const QString &website)
//would not need this as this gets the plasma layout and i just need the theme
{
    const QString metadataPath(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) % QLatin1String("/plasma/look-and-feel/") % pluginName % QLatin1String("/metadata.desktop"));
    KConfig c(metadataPath);

    KConfigGroup cg(&c, "Desktop Entry");
    cg.writeEntry("Name", name);
    cg.writeEntry("Comment", comment);
    cg.writeEntry("X-KDE-PluginInfo-Name", pluginName);
    cg.writeEntry("X-KDE-ServiceTypes", "Plasma/LookAndFeel");
    cg.writeEntry("X-KDE-PluginInfo-Author", author);
    cg.writeEntry("X-KDE-PluginInfo-Email", email);
    cg.writeEntry("X-KDE-PluginInfo-Website", website);
    cg.writeEntry("X-KDE-PluginInfo-Category", "Plasma Look And Feel");
    cg.writeEntry("X-KDE-PluginInfo-License", license);
    cg.writeEntry("X-KDE-PluginInfo-EnabledByDefault", "true");
    cg.writeEntry("X-KDE-PluginInfo-Version", "0.1");
    cg.sync();

    dumpDefaultsConfigFile(pluginName);
}

void Utils::dumpDefaultsConfigFile(const QString &pluginName)
{
    //write the defaults file, read from kde config files and save to the defaultsrc
    KConfig defaultsConfig(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) % QLatin1String("/plasma/look-and-feel/") % pluginName % "/contents/defaults");

    KConfigGroup defaultsConfigGroup(&defaultsConfig, "kdeglobals");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "KDE");

    //widget style
    KConfigGroup systemCG(KSharedConfig::openConfig(QStringLiteral("kdeglobals")), "KDE");
    defaultsConfigGroup.writeEntry("widgetStyle", systemCG.readEntry("widgetStyle", QStringLiteral("breeze")));

    //icon style
    defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kdeglobals");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "Icons");
    systemCG = KConfigGroup(KSharedConfig::openConfig(QStringLiteral("kdeglobals")), "Icons");
    defaultsConfigGroup.writeEntry("Theme", systemCG.readEntry("Theme", QStringLiteral("breeze")));

    defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kdeglobals");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "General");
    systemCG = KConfigGroup(KSharedConfig::openConfig(QStringLiteral("kdeglobals")), "General");
    defaultsConfigGroup.writeEntry("ColorScheme", systemCG.readEntry("ColorScheme", QStringLiteral("Breeze")));

    //plasma theme
    defaultsConfigGroup = KConfigGroup(&defaultsConfig, "plasmarc");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "Theme");
    systemCG = KConfigGroup(KSharedConfig::openConfig(QStringLiteral("plasmarc")), "Theme");
    defaultsConfigGroup.writeEntry("name", systemCG.readEntry("name", QStringLiteral("default")));

    //cursor theme
    defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kcminputrc");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "Mouse");
    systemCG = KConfigGroup(KSharedConfig::openConfig(QStringLiteral("kcminputrc")), "Mouse");
    defaultsConfigGroup.writeEntry("cursorTheme", systemCG.readEntry("cursorTheme", QStringLiteral("breeze_cursors")));

    //KWin window switcher theme
    systemCG = KConfigGroup(KSharedConfig::openConfig(QStringLiteral("kwinrc")), "TabBox");
    defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kwinrc");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "WindowSwitcher");
    defaultsConfigGroup.writeEntry("LayoutName", systemCG.readEntry("LayoutName", QStringLiteral("org.kde.breeze.desktop")));

    defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kwinrc");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "DesktopSwitcher");
    defaultsConfigGroup.writeEntry("LayoutName", systemCG.readEntry("DesktopLayout", QStringLiteral("org.kde.breeze.desktop")));

    systemCG = KConfigGroup(KSharedConfig::openConfig(QStringLiteral("kwinrc")), "org.kde.kdecoration2");
    defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kwinrc");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "org.kde.kdecoration2");
    defaultsConfigGroup.writeEntry("library", systemCG.readEntry("library", QStringLiteral("org.kde.breeze")));
    defaultsConfigGroup.writeEntry("theme", systemCG.readEntry("theme", QString()));
}

void Utils::writeToThemeConfigFile(const QString &pluginName, const QString &themeType)
{
    QString koiPath = QDir::homePath() + QStringLiteral("/.config/koirc");
    //write the defaults file, read from kde config files and save to the defaultsrc
    KConfig defaultsConfig(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) % QLatin1String("/plasma/look-and-feel/") % pluginName % "/contents/defaults");

    KConfigGroup defaultsConfigGroup(&defaultsConfig, "kdeglobals");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "KDE");

    //widget style
    KConfigGroup systemCG(KSharedConfig::openConfig(koiPath), "WidgetStyle");
    defaultsConfigGroup.writeEntry("widgetStyle", systemCG.readEntry(themeType, QStringLiteral("breeze")));

    //icon style
    defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kdeglobals");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "Icons");
    systemCG = KConfigGroup(KSharedConfig::openConfig(koiPath), "IconTheme");
    defaultsConfigGroup.writeEntry("Theme", systemCG.readEntry(themeType, QStringLiteral("breeze")));

    defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kdeglobals");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "General");
    systemCG = KConfigGroup(KSharedConfig::openConfig(koiPath), "ColorScheme");
    defaultsConfigGroup.writeEntry("ColorScheme", systemCG.readEntry(themeType, QStringLiteral("Breeze")));

    //plasma theme
    defaultsConfigGroup = KConfigGroup(&defaultsConfig, "plasmarc");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "Theme");
    systemCG = KConfigGroup(KSharedConfig::openConfig(koiPath), "PlasmaStyle");
    defaultsConfigGroup.writeEntry("name", systemCG.readEntry(themeType, QStringLiteral("default")));

    //cursor theme
    defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kcminputrc");
    defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "Mouse");
    systemCG = KConfigGroup(KSharedConfig::openConfig(koiPath), "Mouse");
    defaultsConfigGroup.writeEntry("cursorTheme", systemCG.readEntry(themeType, QStringLiteral("breeze_cursors")));
}
// Manage switching themes functions
void Utils::useGlobalTheme(QString themeName)
{
    useGlobalProcess = new QProcess;
    QString command = QStringLiteral("lookandfeeltool");
    QStringList arguments = {"-a", std::move(themeName)};
    useGlobalProcess->start(command, arguments);
}
void Utils::goLight()
{
    goLightKvantumStyle(); //this should come before useGlobaltheme for it to update on the fly
    useGlobalTheme("Koi-Light");
    goLightGtk();
    goLightWall();

    if (settings->value("notify").toBool())
    {
        notify("Switched to light mode!", "Some applications may need to be restarted for applied changes to take effect.");
    }
}
void Utils::goDark()
{
    goDarkKvantumStyle(); //this should come before useGlobaltheme for it to update on the fly
    useGlobalTheme("Koi-Dark");
    goDarkGtk();
    goDarkWall();
    if (settings->value("notify").toBool())
    {
        notify("Switched to dark mode!", "Some applications may need to be restarted for applied changes to take effect.");
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
