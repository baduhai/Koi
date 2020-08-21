
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


QStringList Utils::getCursorThemes()
{
	QDir cursorOldLocalParentDir(QDir::homePath() + QStringLiteral("/.local/share/icons/"));
	QDir cursorLocalParentDir(QDir::homePath() + QStringLiteral("/.icons"));
	QDir cursorSystemParentDir(QStringLiteral("/usr/share/icons"));
	QFileInfoList parentDir(cursorOldLocalParentDir.entryInfoList(QDir::Dirs));
	parentDir.append(cursorSystemParentDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot));
	QStringList cursorThemes;
	//oldlocal and system
	for (QFileInfo& info : parentDir)
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
	if (dir.exists())
	{
		sysLib = dir;
	}
	QFileInfoList libInfoTheme = sysLib.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
	QStringList libThemes;
	for (const auto& file : libInfoTheme)
	{
		libThemes.append(file.baseName());
	}
	if (libThemes.contains("kwin5_aurorae"))
	{
		libThemes.removeAt(libThemes.indexOf("kwin5_aurorae"));
	}
	for (auto& theme : libThemes)
	{
		if (theme.endsWith("decoration", Qt::CaseInsensitive))
		{
			theme.chop(10);
		}
	}
	//for aurorae styles
	QStringList auroraeStyles;
	QDir aurLocalLib(QDir::homePath() + "/.local/share/aurorae/themes");
	QDir aurSysLib("/usr/share/aurorae/themes/");
	if (aurSysLib.exists())
	{
		auroraeStyles.append(aurSysLib.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
	}
	if (aurLocalLib.exists())
	{
		auroraeStyles.append(aurLocalLib.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
	}
	auroraeStyles.removeDuplicates();

	for (const auto& th: auroraeStyles)
	{
		Decoration d{};
		d.name = th;
		d.library = "org.kde.kwin.aurorae";
		d.theme = "__aurorae__svg__" + th;
		dt.append(d);
	}
	for (const auto& th: libThemes)
	{
		Decoration d{};
		d.name = th;
		d.name.replace(0, 1, d.name[0].toUpper());
		d.library = "org.kde." + d.name.toLower();
		dt.append(d);
	}
	return dt;
}
bool Utils::themeExists(const QString& themeName)
{
	QFileInfo localTheme(QDir::homePath() + QStringLiteral("/.local/share/plasma/look-and-feel/") + themeName
		+ QStringLiteral("/contents/defaults"));
	return localTheme.exists() && localTheme.isFile();
}
void Utils::createNewTheme(const QString& pluginName,
	const QString& name,
	const QString& comment,
	const QString& author,
	const QString& email,
	const QString& license,
	const QString& website)
//would not need this as this gets the plasma layout and i just need the theme
{
	const QString metadataPath
		(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) % QLatin1String("/plasma/look-and-feel/")
			% pluginName % QLatin1String("/metadata.desktop"));
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

void Utils::dumpDefaultsConfigFile(const QString& pluginName)
{
	//write the defaults file, read from kde config files and save to the defaultsrc
	KConfig defaultsConfig
		(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) % QLatin1String("/plasma/look-and-feel/")
			% pluginName % "/contents/defaults");

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
	defaultsConfigGroup
		.writeEntry("LayoutName", systemCG.readEntry("LayoutName", QStringLiteral("org.kde.breeze.desktop")));

	defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kwinrc");
	defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "DesktopSwitcher");
	defaultsConfigGroup
		.writeEntry("LayoutName", systemCG.readEntry("DesktopLayout", QStringLiteral("org.kde.breeze.desktop")));

	systemCG = KConfigGroup(KSharedConfig::openConfig(QStringLiteral("kwinrc")), "org.kde.kdecoration2");
	defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kwinrc");
	defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "org.kde.kdecoration2");
	defaultsConfigGroup.writeEntry("library", systemCG.readEntry("library", QStringLiteral("org.kde.breeze")));
	defaultsConfigGroup.writeEntry("theme", systemCG.readEntry("theme", QString()));
}

void Utils::writeToThemeConfigFile(const QString& pluginName, const QString& themeType)
{
	QString koiPath = QDir::homePath() + QStringLiteral("/.config/koirc");
	//write the defaults file, read from kde config files and save to the defaultsrc
	KConfig defaultsConfig
		(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) % QLatin1String("/plasma/look-and-feel/")
			% pluginName % "/contents/defaults");

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

	//window decoration
	defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kwinrc");
	defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "org.kde.kdecoration2");
	systemCG = KConfigGroup(KSharedConfig::openConfig(koiPath), "WindowDecoration");
	defaultsConfigGroup
		.writeEntry("library", systemCG.readEntry(themeType + "Library", QStringLiteral("org.kde.breeze")));
	defaultsConfigGroup.writeEntry("theme", systemCG.readEntry(themeType + "Theme", QString()));
}

// Manage switching themes functions
void Utils::useGlobalTheme(QString themeName)
{
	useGlobalProcess = new QProcess;
	QString command = QStringLiteral("lookandfeeltool");
	QStringList arguments = { "-a", std::move(themeName) };
	useGlobalProcess->start(command, arguments);
}

//TODO to simplify this and remove redundant methods
void Utils::goLight()
{
	goKvantumStyle("light"); //this should come before useGlobaltheme for it to update on the fly
	useGlobalTheme("Koi-Light");
	goColors("light");
	goGtk("light");
	goWall("light");
	runScript("light");

	if (settings->value("notify").toBool())
	{
		notify("Switched to light mode!",
			"Some applications may need to be restarted for applied changes to take effect.");
	}
}
void Utils::goDark()
{
	goKvantumStyle("dark"); //this should come before useGlobaltheme for it to update on the fly
	useGlobalTheme("Koi-Dark");
	goColors("dark");
	goGtk("dark");
	goWall("dark");
	runScript("dark");
	if (settings->value("notify").toBool())
	{
		notify("Switched to dark mode!",
			"Some applications may need to be restarted for applied changes to take effect.");
	}
}

void Utils::goColors(const QString &themeType)
{
if (settings->value("ColorScheme/enabled").toBool())
	{
		colorScheme.setColorScheme(settings->value("ColorScheme/" + themeType ).toString());
	}
}

void Utils::goGtk(const QString &themeType)
{
	if (settings->value("GTKTheme/enabled").toBool())
	{
		gtk.setGtk(settings->value("GTKTheme/" + themeType).toString());
	}
}

void Utils::goKvantumStyle(const QString &themeType)
{
	if (settings->value("KvantumStyle/enabled").toBool())
	{
		kvantumStyle.setKvantumStyle(settings->value("KvantumStyle/" + themeType).toString());
	}
}

void Utils::goWall(const QString &themeType)
{
	if (settings->value("Wallpaper/enabled").toBool())
	{
		if (!settings->value("Wallpaper/" + themeType).isNull())
		{
			wallpaper.setWallpaper(settings->value("Wallpaper/" + themeType ).toString());
		}
		else
		{
			notify("Error setting Wallpaper",
				"Koi tried to change your " + themeType +  " wallpaper, but no wallpaper fie was selected",
				0);
		}
	}
}

QStringList Utils::getWindowDecorationsStyle()
{
	QList<Decoration> dt = Utils::getWindowDecorations();
	QStringList styleList;
	for (const auto& style: dt)
	{
		styleList.append(style.name);
	}
	styleList.sort();
	return styleList;
}

void Utils::runScript(const QString& themeType)
{

	if (settings->value("Script/" + themeType + "Enabled").toBool())
	{
		if (!QProcess::startDetached("/bin/sh", QStringList{ settings->value("Script/" + themeType).toString() }))
	 	{
			qDebug() << "Failed to run " + themeType + " script";
		}
	}
}

