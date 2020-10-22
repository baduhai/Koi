//
// Created by da-viper on 05/09/2020.
//

#include "profile.h"

//This is used for getting the current active plasma style settings
//and setting it as default.
Profile::Profile()
	:
	m_name(), m_pluginName(), m_plasma(), m_color(), //name=themeName pluginName= koi-themeName
	m_gtk(), m_kvantum(), m_widget(),
	m_icon(), m_mouse(), m_script(),m_scriptEnabled(false), m_wallpaper(), m_wallEnabled(false),
	m_library(), m_theme(), m_konsole(), m_konsoleEnabled()
{

	// KSharedconfig would source from the QStandardPath::configLocation if no path is given.
	//plasma theme
	KConfigGroup systemCG(KSharedConfig::openConfig(QStringLiteral("plasmarc")), "Theme");
	m_plasma = systemCG.readEntry("name", QStringLiteral("default"));
	systemCG = KConfigGroup(KSharedConfig::openConfig(QStringLiteral("kdeglobals")), "KDE");
	m_widget = systemCG.readEntry("widgetStyle", QStringLiteral("breeze"));
	//color
	systemCG = KConfigGroup(KSharedConfig::openConfig(QStringLiteral("kdeglobals")), "General");
	m_color = systemCG.readEntry("ColorScheme", QStringLiteral("Breeze"));
	//icon
	systemCG = KConfigGroup(KSharedConfig::openConfig(QStringLiteral("kdeglobals")), "Icons");
	m_icon = systemCG.readEntry("Theme", QStringLiteral("breeze"));
	//mouse
	systemCG = KConfigGroup(KSharedConfig::openConfig(QStringLiteral("kcminputrc")), "Mouse");
	m_mouse = systemCG.readEntry("cursorTheme", QStringLiteral("breeze_cursors"));
	//decoration library and theme.
	systemCG = KConfigGroup(KSharedConfig::openConfig(QStringLiteral("kwinrc")), "org.kde.kdecoration2");
	m_library = systemCG.readEntry("library", QStringLiteral("org.kde.breeze"));
	m_theme = systemCG.readEntry("theme", QString());

}

//copying from and existing profile.
Profile::Profile(const Profile &p)
	:
	m_name(), m_pluginName("Koi-" + m_name), m_plasma(p.m_plasma), m_color(p.m_color),
	m_gtk(p.m_gtk), m_kvantum(p.m_kvantum), m_widget(p.m_widget),
	m_icon(p.m_icon), m_mouse(p.m_mouse), m_script(p.m_script),m_scriptEnabled(p.m_scriptEnabled),
	m_wallpaper(p.m_wallpaper), m_wallEnabled(p.m_wallEnabled),
	m_library(p.m_library), m_theme(p.m_theme), m_konsole(p.m_konsole)
{
	m_name = p.m_name + "-1";
	m_pluginName = "Koi-" + m_name;
}

Profile::~Profile()
{

}

void Profile::readConfig(QSettings &s)
{

	//Style
	s.beginGroup("Style");
	m_plasma = s.value("plasmaStyle").toString();
	m_color = s.value("colorScheme").toString();
	m_gtk = s.value("gtkTheme").toString();
	m_kvantum = s.value("kvantum").toString();
	m_widget = s.value("widgetStyle").toString();


//	qDebug() << "the setting param " << s.value("plasmaStyle").toString() << s.value("colorScheme").toString()
//			 << s.value("kvantum").toString();
	s.endGroup();
	//Others;
	s.beginGroup("Others");
	m_icon = s.value("icon").toString();
	m_mouse = s.value("mouse").toString();
	m_script = s.value("script").toString();
	m_scriptEnabled = s.value("scriptEnabled").toBool();
	m_wallpaper = s.value("wallpaper").toString();
	m_wallEnabled = s.value("wallEnabled").toBool();
	s.endGroup();

	//Window Decoration
	s.beginGroup("Window Decoration");
	m_decorationName = s.value("name").toString();
	m_library = s.value("library").toString();
	m_theme = s.value("theme").toString();
	s.endGroup();

	//External
	s.beginGroup("External");
	m_konsole = s.value("konsole").toString();
	m_konsoleEnabled = s.value(QStringLiteral("konsoleEnabled")).toBool();
	s.endGroup();
}

void Profile::writeConfig(QSettings &s) const
{
	//TODO only replace what has changed.
	//void write to globalTheme.
	//Styles
	s.beginGroup("Style");
	s.setValue("plasmaStyle", m_plasma);
	s.setValue("colorScheme", m_color);
	s.setValue("gtkTheme", m_gtk);
	s.setValue("kvantum", m_kvantum);
	s.setValue("widgetStyle", m_widget);
	s.endGroup();

	//Others
	s.beginGroup("Others");
	s.setValue("icon", m_icon);
	s.setValue("mouse", m_mouse);
	s.setValue("script", m_script);
	s.setValue("scriptEnabled", m_scriptEnabled);
	s.setValue("wallpaper", m_wallpaper);
	s.setValue("wallEnabled", m_wallEnabled);
	s.endGroup();

	//Window Decorations;
	s.beginGroup("Window Decoration");
	s.setValue("name", m_decorationName);
	s.setValue("library", m_library);
	s.setValue("theme", m_theme);
	s.endGroup();

	//External
	s.beginGroup("External");
	s.setValue("konsole", m_konsole);
    s.setValue(QStringLiteral("konsoleEnabled"),m_konsoleEnabled);
	s.endGroup();
}

/*  data written to the below location is never read by this app
    *it is for applying kde specific styles with the command lookandfeel
    *theme global configs is always prefixed with Koi-$nameoftheme
*/
void Profile::writeToGlobal() const
{
	QString pluginName(m_pluginName);
	//write the defaults file, read from kde config files and save to the defaultsrc
	KConfig defaultsConfig
		(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) %
			QLatin1String("/plasma/look-and-feel/")
			 % pluginName % "/contents/defaults");

	KConfigGroup defaultsConfigGroup(&defaultsConfig, "kdeglobals");
	defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "KDE");

	//widget style
	defaultsConfigGroup.writeEntry("widgetStyle", m_widget);

	//icon style
	defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kdeglobals");
	defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "Icons");
	defaultsConfigGroup.writeEntry("Theme", m_icon);

	//colour style
	defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kdeglobals");
	defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "General");
	defaultsConfigGroup.writeEntry("ColorScheme", m_color);

	//plasma theme
	defaultsConfigGroup = KConfigGroup(&defaultsConfig, "plasmarc");
	defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "Theme");
	defaultsConfigGroup.writeEntry("name", m_plasma);

	//cursor theme
	defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kcminputrc");
	defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "Mouse");
	defaultsConfigGroup.writeEntry("cursorTheme", m_mouse);

	//window decoration
	defaultsConfigGroup = KConfigGroup(&defaultsConfig, "kwinrc");
	defaultsConfigGroup = KConfigGroup(&defaultsConfigGroup, "org.kde.kdecoration2");
	defaultsConfigGroup.writeEntry("library", m_library);
	defaultsConfigGroup.writeEntry("theme", m_theme);
}
void Profile::setName(const QString &name)
{
	m_name = name;
	m_pluginName = "Koi-" + m_name;
}
QString Profile::name() const
{
	return m_name;
}
QString Profile::pluginName() const
{
	return m_pluginName;
}
bool Profile::globalDefaultExists() const
{
	QFileInfo localTheme(getGlobDir() + QStringLiteral("/contents/defaults"));
	return localTheme.exists() && localTheme.isFile();
}
void Profile::createProfileGlobalDir() const
//would not need this as this gets the plasma layout and i just need the theme
{
	const QString metadataPath
		(getGlobDir() % QLatin1String("/metadata.desktop"));
	KConfig c(metadataPath);

	KConfigGroup cg(&c, "Desktop Entry");
	cg.writeEntry("Name", m_pluginName);
	cg.writeEntry("Comment", "This is automatically created from koi app ");
	cg.writeEntry("X-KDE-PluginInfo-Name", m_pluginName);
	cg.writeEntry("X-KDE-ServiceTypes", "Plasma/LookAndFeel");
	cg.writeEntry("X-KDE-PluginInfo-Author", "Da-Viper and  Baduhai");
	cg.writeEntry("X-KDE-PluginInfo-Email", "yerimyah1@gmail.com");
	cg.writeEntry("X-KDE-PluginInfo-Website", "https://github.com/Da-Viper/Koi");
	cg.writeEntry("X-KDE-PluginInfo-Category", "Plasma Look And Feel");
	cg.writeEntry("X-KDE-PluginInfo-License", "LGPL-3.0");
	cg.writeEntry("X-KDE-PluginInfo-EnabledByDefault", "true");
	cg.writeEntry("X-KDE-PluginInfo-Version", "0.1");
	cg.sync();

}

QString Profile::getGlobDir() const
{
	return m_globDir.absolutePath();
}
QString Profile::configPath() const
{
	return m_configPath;
}
//Getters
QString Profile::getPlasma() const
{
	return m_plasma;
}
QString Profile::getColor() const
{
	return m_color;
}
QString Profile::getGtk() const
{
	return m_gtk;
}
QString Profile::getKvantum() const
{
	return m_kvantum;
}
QString Profile::getWidget() const
{
	return m_widget;
}
QString Profile::getIcon() const
{
	return m_icon;
}
QString Profile::getMouse() const
{
	return m_mouse;
}
QString Profile::getScript() const
{
	return m_script;
}
bool Profile::getScriptEnabled() const
{
	return m_scriptEnabled;
}
QVariant Profile::getWallpaper() const
{
	return m_wallpaper;
}
bool Profile::getWallEnabled() const
{
	return m_wallEnabled;
}
QString Profile::getDecName() const
{
	return m_decorationName;
}

QString Profile::getKonsole() const
{
	return m_konsole;
}
bool Profile::getKonsoleEnabled() const
{
    return m_konsoleEnabled;
}
//Setters
void Profile::setPlasma(const QString &plasma)
{
	m_plasma = plasma;
}
void Profile::setColor(const QString &color)
{
	m_color = color;

}
void Profile::setGtk(const QString &gtk)
{
	m_gtk = gtk;
}
void Profile::setKvantum(const QString &kvantum)
{
	m_kvantum = kvantum;
}
void Profile::setWidget(const QString &widget)
{
	m_widget = widget;
}
void Profile::setIcon(const QString &icon)
{
	m_icon = icon;
}
void Profile::setMouse(const QString &mouse)
{
	m_mouse = mouse;
}
void Profile::setScript(const QString &script)
{
	m_script = script;
}
void Profile::setScriptEnabled(const bool &scriptEnabled)
{
	m_scriptEnabled = scriptEnabled;
}
void Profile::setWallpaper(QVariant wallpaper)
{
	m_wallpaper = wallpaper;
}
void Profile::setWallEnabled(const bool &wallEnabled)
{
	m_wallEnabled = wallEnabled;
}
void Profile::setDecName(const QString &decorationName)
{
	m_decorationName = decorationName;
}
void Profile::setLibrary(const QString &library)
{
	m_library = library;
}
void Profile::setTheme(const QString &theme)
{
	m_theme = theme;
}
void Profile::setKonsole(const QString &konsole)
{
	m_konsole = konsole;
}
void Profile::setKonsoleEnabled(const bool &konsoleEnabled)
{
    m_konsoleEnabled = konsoleEnabled;
}
void Profile::setGlobDir()
{
	m_globDir.setPath(
		QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) % QLatin1String("/plasma/look-and-feel/")
			% m_pluginName);
}
void Profile::setConfigPath()
{
	m_configPath = QStandardPaths::writableLocation(
		QStandardPaths::GenericDataLocation) + "/koi/" + m_name + ".koi";
}
