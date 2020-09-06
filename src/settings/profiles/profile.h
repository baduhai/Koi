//
// Created by da-viper on 05/09/2020.
//

#ifndef KOI_PROFILE_H
#define KOI_PROFILE_H

#include <QString>
#include <QSettings>
#include <KConfig>
#include <KConfigGroup>
#include <QDir>
#include <KSharedConfig>

class Profile
{

public:
	Profile();
	Profile(const Profile &p);
	~Profile();

	void readConfig(QSettings *s);

	void writeConfig(QSettings *s) const;

	//cannot use qsetting here because of double grouping in defaults file ;
	void writeToGlobal() const;

	// Sets the Profile Name
	void setName(const QString &name);
	//QString m_browser;
	void createProfileGlobalDir() const;

	//QString m_vscode;
	static bool globalDefaultExists(const QString &pluginName);
	// Returns the profile Name
	QString name() const;
private:

	//name of the profile that will be used for its location
	QString m_name;
	//can never set the plugin in name manually;
	QString m_pluginName;
	//Group
	//[Styles]
	QString m_plasma;
	QString m_color;
	QString m_gtk;

	QString m_kvantum;
	QString m_widget;
	//[Others]
	QString m_icon;
	QString m_mouse;

	QString m_script;
	QString m_wallpaper;

	//[Window Decorations];
	QString m_library;
	QString m_theme;

	//External ;
	QString m_konsole;
};


#endif //KOI_PROFILE_H
