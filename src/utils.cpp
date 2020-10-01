
#include "utils.h"

Utils::Utils()
{
}
Utils::Utils(Profile *pProfile)
{
	Q_ASSERT(pProfile);
	_profile = pProfile;

}

// Miscelaneous functions
void Utils::notify(QString notifySummary, QString notifyBody, int timeoutms) // Push notification through DBus
{
	bus = new QDBusConnection(QDBusConnection::sessionBus());
	notifyInterface = new QDBusInterface("org.freedesktop.Notifications",
										 "/org/freedesktop/Notifications",
										 "org.freedesktop.Notifications",
										 *bus);
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
	notifyInterface->call("Notify", app_name, replaces_id, app_icon, summary, body, actions, hints, timeout);
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

//PlasmaStyle

QStringList Utils::getPlasmaStyles() // Get all available plasma styles
{
	QDir stylesLocalDir(QDir::homePath() + "/.local/share/plasma/desktoptheme");
	QDir stylesSystemDir("/usr/share/plasma/desktoptheme");
	QStringList plasmaStyles;
	if (stylesLocalDir.exists()) {
		plasmaStyles.append(stylesLocalDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
	}
	if (stylesSystemDir.exists()) {
		plasmaStyles.append(stylesSystemDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
	}
	plasmaStyles.removeDuplicates();
	plasmaStyles.append("breeze");
	return plasmaStyles;
}

//Color Schemes
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
	return colorSchemesNames;
}

//GTK
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
//widget styles
QStringList Utils::getWidgetStyles()
{
	//this literally took me 2 hrs to find.
	QStringList widgetStyles = QStyleFactory::keys();
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
	return kvantumStyles;
}

//Icons
QStringList Utils::getIcons() // Get all available icon themes
{
	QDir iconsLocalDir(QDir::homePath() + "/.local/share/icons");
	QDir iconsSystemDir("/usr/share/icons");
	QStringList iconThemes = iconsLocalDir.entryList(QDir::Dirs) + iconsSystemDir.entryList(QDir::Dirs);
	iconThemes.removeDuplicates();
	iconThemes.removeFirst();
	iconThemes.removeFirst();
	return iconThemes;
}

//cursor
QStringList Utils::getCursorThemes()
{
	QDir cursorOldLocalParentDir(QDir::homePath() + QStringLiteral("/.local/share/icons/"));
	QDir cursorLocalParentDir(QDir::homePath() + QStringLiteral("/.icons"));
	QDir cursorSystemParentDir(QStringLiteral("/usr/share/icons"));
	QFileInfoList parentDir(cursorOldLocalParentDir.entryInfoList(QDir::Dirs));
	parentDir.append(cursorSystemParentDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot));
	QStringList cursorThemes;
	//oldlocal and system
	for (QFileInfo &info : parentDir) {
		QDir filepath(info.absoluteFilePath() + QStringLiteral("/cursors"));
		if (filepath.exists()) {
			cursorThemes.append(info.fileName());
		}
	}
	//local cursors
	cursorThemes.append(cursorLocalParentDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
	return cursorThemes;
}

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
	QDir dir("/usr/lib/qt/plugins/org.kde.kdecoration2/");
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

bool Utils::themeExists(const QString &themeName)
{
	QFileInfo localTheme(QDir::homePath() + QStringLiteral("/.local/share/plasma/look-and-feel/") + themeName
							 + QStringLiteral("/contents/defaults"));
	return localTheme.exists() && localTheme.isFile();
}

// Manage switching themes functions
void Utils::useGlobalTheme()
{
	QString command = QStringLiteral("lookandfeeltool");
	QStringList arguments = {"-a", _profile->pluginName()};
	auto *useGlob = new QProcess();
	useGlob->start(command, arguments);
	//TODO recheck this there was memoryleak
	QObject::connect(useGlob, qOverload<int,QProcess::ExitStatus>(&QProcess::finished),useGlob, &QProcess::deleteLater);
}

// Use to switch to a different theme profile
void Utils::go()
{
	goKvantumStyle();
	useGlobalTheme();
	goGtk();
	goWall();
	runScript();
	QSettings s;
	if (s.value("notify").toBool()) {
		notify("Switched to " + _profile->name() + " mode!",
			   "Some applications may need to be restarted for applied changes to take effect.");
	}
}

void Utils::goGtk()
{
    QString s(_profile->getGtk() );
	gtk.setGtk(s);
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
		//TODO check if it is an actually file and a picture.
		wallpaper.setWallpaper(_profile->getWallpaper());
	}
	else {
		//TODO change the type of error.
		notify("Error setting Wallpaper",
			   "Koi tried to change your " + _profile->name() + " wallpaper, but no wallpaper fie was selected",
			   5000);
	}
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
void Utils::runScript()
{
	if (_profile->getScriptEnabled()) {
		if (!QProcess::startDetached("/bin/sh", {_profile->getScript()})) {
			qDebug() << "Failed to run " + _profile->name() + " script";
		}
	}
}




