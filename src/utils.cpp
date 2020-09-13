
#include "utils.h"

Utils::Utils()
{
}

// Global settings stuff
void Utils::initialiseSettings()
{
	//settings = new QSettings("koirc", QSettings::IniFormat); // Line used for testing !Must comment before pushing!
	//TODO switch to koiglobalrc because a file is already using koirc (Koirc)
	settings =
		new QSettings(QDir::homePath() + "/.config/koirc", QSettings::IniFormat); // Setting config path and format
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
void Utils::startupTimeCheck() // Check if switching is needed based on time.
{
	QTime lightTime = QTime::fromString(settings->value("time-light").toString(), "hh:mm:ss");
	QTime darkTime = QTime::fromString(settings->value("time-dark").toString(), "hh:mm:ss");
	QTime now = QTime::currentTime();
	if (now < lightTime && now < darkTime) {
		QTest::qWait(1000); // Needed delay, or Koi may use the wrong color scheme.
		go("dark");
	}
	else if (now == lightTime) // Highly unlikely
	{
		QTest::qWait(1000);
		go("light");
	}
	else if (now > lightTime && now < darkTime) {
		QTest::qWait(1000);
		go("light");
	}
	else if (now == darkTime) // Highly unlikely
	{
		QTest::qWait(1000);
		go("dark");
	}
	else {
		QTest::qWait(1000);
		go("dark");
	}
}

// Get stuff
// this will be listed orderly
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

//widget styles
QStringList Utils::getWidgetStyles()
{
	//this literally took me 2 hrs to find.
	QStringList widgetStyles = QStyleFactory::keys();
	return widgetStyles;
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
void Utils::useGlobalTheme(QString themeName)
{
	useGlobalProcess = new QProcess;
	QString command = QStringLiteral("lookandfeeltool");
	QStringList arguments = {"-a", std::move(themeName)};
	useGlobalProcess->start(command, arguments);
}

// Use to switch to a different theme profile
void Utils::go(const QString &themeType)
{
	goKvantumStyle(themeType);
	useGlobalTheme("Koi-" + themeType);
	goColors(themeType);
	goGtk(themeType);
	goWall(themeType);
	runScript(themeType);
	if (settings->value("notify").toBool()) {
		notify("Switched to " + themeType + " mode!",
			   "Some applications may need to be restarted for applied changes to take effect.");
	}
}

void Utils::goColors(const QString &themeType)
{
	if (settings->value("ColorScheme/enabled").toBool()) {
		colorScheme.setColorScheme(settings->value("ColorScheme/" + themeType).toString());
	}
}

void Utils::goGtk(const QString &themeType)
{
	if (settings->value("GTKTheme/enabled").toBool()) {
		gtk.setGtk(settings->value("GTKTheme/" + themeType).toString());
	}
}

void Utils::goKvantumStyle(const QString &themeType)
{
	if (settings->value("KvantumStyle/enabled").toBool()) {
		kvantumStyle.setKvantumStyle(settings->value("KvantumStyle/" + themeType).toString());
	}
}

void Utils::goWall(const QString &themeType)
{
	if (settings->value("Wallpaper/enabled").toBool()) {
		if (!settings->value("Wallpaper/" + themeType).isNull()) {
			wallpaper.setWallpaper(settings->value("Wallpaper/" + themeType).toString());
		}
		else {
			notify("Error setting Wallpaper",
				   "Koi tried to change your " + themeType + " wallpaper, but no wallpaper fie was selected",
				   0);
		}
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

void Utils::runScript(const QString &themeType)
{

	if (settings->value("Script/" + themeType + "Enabled").toBool()) {
		if (!QProcess::startDetached("/bin/sh", QStringList{settings->value("Script/" + themeType).toString()})) {
			qDebug() << "Failed to run " + themeType + " script";
		}
	}
}



