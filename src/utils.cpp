
#include "utils.h"

Utils::Utils(Profile *pProfile)
    :wallpaper()
{
	Q_ASSERT(pProfile);
	_profile = pProfile;

}
Utils::~Utils()
{
}

// Miscelaneous functions
void Utils::notify(QString notifySummary, QString notifyBody, int timeoutms) // Push notification through DBus
{
	QDBusInterface notifyInterface("org.freedesktop.Notifications",
										 "/org/freedesktop/Notifications",
										 "org.freedesktop.Notifications",
										 QDBusConnection::sessionBus());
	QString app_name = "Koi";        // What program is the notification coming from?
	uint replaces_id = 0;            // Not sure what this is. Think it has something to do with pid.
	QString
		app_icon;                // Not actually specifying app icon, this is if you'd like to push an image alog with notification.
	QString summary = std::move(notifySummary); // Title of notification.
	QString body = std::move(notifyBody);       // Notification body.
	QStringList actions;             // No idea how to use.
	QVariantMap hints;               // No idea how to use.
	int timeout =
		timeoutms;         // Notification timeout, there's no way to assume system has a default timeout unfortunately.
	notifyInterface.call("Notify", app_name, replaces_id, app_icon, summary, body, actions, hints, timeout);
}

void Utils::setGtk(const QString& gtkTheme)
{
    QDBusInterface interface("org.kde.GtkConfig", "/GtkConfig", "org.kde.GtkConfig", QDBusConnection::sessionBus());
    interface.call("setGtk2Theme", gtkTheme);
    interface.call("setGtk3Theme", gtkTheme);
}

QString Utils::startupTimeCheck() // get the nearest earlier favourite theme.
{
	QSettings settings;
	settings.beginGroup("Favourites");
	auto favList = settings.allKeys();
	QTime currentTime = QTime::currentTime();
	int nearest = 0;
	QString nearestName;

	for (const auto &favourite: favList) {
		QString timeStr = settings.value(favourite).toString();
		if(timeStr.isEmpty() || timeStr.isNull()){
			continue;
		}
		QTime favTime(QTime::fromString(timeStr));
		int timeSec = currentTime.secsTo(favTime);
		if (timeSec > 0) {
			timeSec -= 86400;
		}
		if (nearest == 0) {
			nearestName = favourite;
			nearest = timeSec;
			continue;
		}
		else if (nearest < 0 && timeSec > 0) {
			continue;
		}
		else if (nearest > timeSec) {
			continue;
		}
		else {
			nearest = timeSec;
			nearestName = favourite;
		}
	}
	return nearestName;
}

//Get all Needed this for a profile.
//PlasmaStyle
QStringList Utils::getPlasmaStyles() // Get all available plasma styles
{
    QStringList plasmaStyles;
    QStringList plasmaDirList(QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, "plasma/desktoptheme", QStandardPaths::LocateDirectory));
    for (const auto &path: plasmaDirList){
        QDir plasmaDir(path);
        if(plasmaDir.exists()){
            plasmaStyles.append(plasmaDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
        }
    }

	plasmaStyles.append("breeze");
    plasmaStyles.removeDuplicates();
    plasmaStyles.sort();
	return plasmaStyles;
}

//Color Schemes
//TODO use standard paths
QStringList Utils::getColorSchemes() // Get all available color schemes
{
	QDir colorsLocalDir(QDir::homePath() + "/.local/share/color-schemes");
	colorsLocalDir.setNameFilters(QStringList() << "*.colors");
	colorsLocalDir.setFilter(QDir::Files);
	colorsLocalDir.setSorting(QDir::Name);
	QList<QFileInfo> colorSchemesLocal = colorsLocalDir.entryInfoList();
	QStringList colorSchemesLocalNames;
	for (const auto &color : qAsConst(colorSchemesLocal)) {
		colorSchemesLocalNames.append(color.baseName());
	}
	QDir colorsSystemDir("/usr/share/color-schemes");
	colorsSystemDir.setNameFilters(QStringList() << "*.colors");
	colorsSystemDir.setFilter(QDir::Files);
	colorsSystemDir.setSorting(QDir::Name);
	QList<QFileInfo> colorSchemesSystem = colorsSystemDir.entryInfoList();
	QStringList colorSchemesSystemNames;
	for (const auto &i : qAsConst(colorSchemesSystem)) {
		colorSchemesSystemNames.append(i.baseName());
	}
	QStringList colorSchemesNames = colorSchemesSystemNames + colorSchemesLocalNames;
	colorSchemesNames.sort();
	return colorSchemesNames;
}

//GTK
QStringList Utils::getGtkThemes() // Get all available gtk themes
{
    QStringList gtkThemes;
    QString gtkLocalDir(QDir::homePath() + QStringLiteral("/.themes"));
    //all the possible path of gtkthemes.
    QStringList gtkDirList(QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, "themes", QStandardPaths::LocateDirectory));
    gtkDirList.append(gtkLocalDir);
    for( const auto &path: gtkDirList){
        QDir gtkDir(path);
        if(!gtkDir.exists()){
            continue;
        }
        auto themeList(gtkDir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot));

        for(const auto &tName: themeList){
            QDir themeDir(tName.absoluteFilePath());
            if(themeDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).contains(QStringLiteral("gtk-3.0"))){
                gtkThemes.append(themeDir.dirName());
            }
        }
    }
	gtkThemes.removeDuplicates();
	gtkThemes.sort();
	return gtkThemes;
}
//widget styles
QStringList Utils::getWidgetStyles()
{
	//this literally took me 2 hrs to find.
	QStringList widgetStyles = QStyleFactory::keys();
	widgetStyles.sort();
	return widgetStyles;
}
//Kvantum
QStringList Utils::getKvantumStyles() // Get all available kvantum styles
{
	QDir kvantumStyleLocalDir(QDir::homePath() + "/.config/Kvantum");
	QDir kvantumStyleSystemDir("/usr/share/Kvantum");
	QStringList kvantumStyles;
	if (kvantumStyleLocalDir.exists()) {
		kvantumStyles.append(kvantumStyleLocalDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
	}
	if (kvantumStyleSystemDir.exists()) {
		kvantumStyles.append(kvantumStyleSystemDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
	}
	kvantumStyles.removeDuplicates();
	kvantumStyles.sort();
	return kvantumStyles;
}

//Icons
//TODO some of this are not actually icon themes like hicolor /remove them
QStringList Utils::getIcons() // Get all available icon themes
{
    QStringList iconDirList(QStandardPaths::locateAll(QStandardPaths::GenericDataLocation,
                                                      "icons", QStandardPaths::LocateDirectory));
    QStringList iconThemes;

    for( const auto &path: iconDirList){
        QDir iconDir(path);
        if(!iconDir.exists()){
            continue;
        }
        auto themeList(iconDir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot));
        for(const auto &tName: themeList){
            QDir themeDir(tName.absoluteFilePath());
            if(themeDir.entryList(QDir::Files | QDir::NoDotAndDotDot).contains(QStringLiteral("index.theme"))){
                iconThemes.append(themeDir.dirName());
            }
        }
    }
    //TODO a better way
    iconThemes.removeAll(QStringLiteral("hicolor"));
    iconThemes.removeAll(QStringLiteral("default"));
	iconThemes.sort();
	return iconThemes;
}

//cursor
QStringList Utils::getCursorThemes()
{
    QStringList cursorThemes;
    QString cursorLocalDir(QDir::homePath() + QStringLiteral("/.icons"));
    //all the possible path of cursors.
    QStringList cursorDirList(QStandardPaths::locateAll(QStandardPaths::GenericDataLocation,
                                                        "icons", QStandardPaths::LocateDirectory));
    cursorDirList.append(cursorLocalDir);
    for( const auto &path: cursorDirList){
        QDir cursorDir(path);
        if(!cursorDir.exists()){
            continue;
        }
        auto cursorInfoList(cursorDir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot));
        for(const auto &tName: cursorInfoList){
            QDir themeDir(tName.absoluteFilePath());
            if(themeDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).contains(QStringLiteral("cursors"))){
                cursorThemes.append(themeDir.dirName());
            }
        }
    }

	cursorThemes.sort();
	return cursorThemes;
}

//Window Decorations.
QList<Decoration> Utils::getWindowDecorations()
{
	/*There are two ways that window decoration is written that i know of
	using the
	"library = org.kde.nameofthelibrary"  then the "theme name" stored in /usr/lib/qt/plugins/org.kde.kdecoration2/  or
	"library =library=org.kde.kwin.aurorae"  then "theme=__aurorae__svg__nameoftheme"
	but i am not sure how to get the actual name of the library and the theme it uses
	its kinda hacky but i did it this way , if there is a better way tell me and i would update this  */
	QList<Decoration> dt;
	QDir sysLib; //for the library

	QString pluginDir(QLibraryInfo::location(QLibraryInfo::PluginsPath));
	QDir dir(pluginDir + "/org.kde.kdecoration2/");
	//TODO delete path variable just for debugging
	QString path(dir.absolutePath());
	qDebug() << "the plugin path " <<  path;
	if (dir.exists()) {
		sysLib = dir;
	}
	QFileInfoList libInfoTheme = sysLib.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
	QStringList libThemes;
	for (const auto &file : qAsConst(libInfoTheme)) {
		libThemes.append(file.baseName());
	}
	if (libThemes.contains("kwin5_aurorae")) {
		libThemes.removeAt(libThemes.indexOf("kwin5_aurorae"));
	}
	for (auto &theme : libThemes) {
		if (theme.endsWith("decoration", Qt::CaseInsensitive)) {
			theme.chop(10);
		}
	}
	//for aurorae styles
	QStringList auroraeStyles;
	QDir aurLocalLib(QDir::homePath() + "/.local/share/aurorae/themes");
	QDir aurSysLib("/usr/share/aurorae/themes/");
	if (aurSysLib.exists()) {
		auroraeStyles.append(aurSysLib.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
	}
	if (aurLocalLib.exists()) {
		auroraeStyles.append(aurLocalLib.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
	}
	auroraeStyles.removeDuplicates();

	for (const auto &th: auroraeStyles) {
		Decoration d{};
		d.name = th;
		d.library = "org.kde.kwin.aurorae";
		d.theme = "__aurorae__svg__" + th;
		dt.append(d);
	}
	for (const auto &th: libThemes) {
		Decoration d{};
		d.name = th;
		d.name.replace(0, 1, d.name[0].toUpper());
		d.library = "org.kde." + d.name.toLower();
		dt.append(d);
	}
	return dt;
}
QStringList Utils::getWindowDecorationsStyle()
{
    QList<Decoration> dt = Utils::getWindowDecorations();
    QStringList styleList;
    for (const auto &style: qAsConst(dt)) {
        styleList.append(style.name);
    }
    styleList.sort();
    return styleList;
}

// Use to switch to a different theme profile
void Utils::go()
{
    QSettings s;
    goKvantumStyle();
    useGlobalTheme();
    setGtk(_profile->getGtk());
    goWall();
    runScript();

    //update colours;
    auto *krdbProcess = new QProcess();
    krdbProcess->setProgram(QStringLiteral("krdb"));
    QObject::connect(krdbProcess, qOverload<int,QProcess::ExitStatus>(&QProcess::finished),krdbProcess, &QProcess::deleteLater);
    krdbProcess->start();
    if (s.value("notify").toBool()) {
        notify("Switched to " + _profile->name() + " mode!",
               "Some applications may need to be restarted for applied changes to take effect.");
    }
}

// Manage switching plasma themes
void Utils::useGlobalTheme()
{
    auto *useGlob = new QProcess();
    QString command = QStringLiteral("lookandfeeltool");
    QStringList arguments = {QStringLiteral("-a"), _profile->pluginName()};
    /** UPDATE it is a bug in QT version 5.15.1
     * i don't know why if i use the below command i get
     * SIGTRAP error when debugging if above any of the functions that calls Dbus
     * in the Utils::go() function above.
     */
    QObject::connect(useGlob,
                     qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
                     useGlob,
                     &QProcess::deleteLater);
    useGlob->start(command, arguments);
//    if (!QProcess::startDetached(command, arguments)) {
//			qDebug() << "Failed to run " + _profile->name() + " Global theme";
//		}
}

void Utils::goKvantumStyle()
{
	QString widget(_profile->getWidget());
	if (widget == "kvantum" || widget == "kvantum-dark") {
		kvantumStyle.setKvantumStyle(_profile->getKvantum());
	}
}

void Utils::goWall()
{
	if (_profile->getWallEnabled()) {
	    wallpaper.setWallpaper(_profile->getWallpaper());
	}
	else {
		notify("Error setting Wallpaper",
			   "Koi tried to change your " + _profile->name() + " wallpaper, but no wallpaper fie was selected",
			   5000);
	}
}
void Utils::runScript()
{
	if (_profile->getScriptEnabled()) {
		if (!QProcess::startDetached("/bin/sh", {_profile->getScript()})) {
			qDebug() << "Failed to run " + _profile->name() + " script";
		}
	}
}




