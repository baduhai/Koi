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

	//getters and setters for the variables
	//getters
	QString getPlasma() const;
	QString getColor() const;
	QString getGtk() const;
	QString getKvantum() const;
	QString getWidget() const;
	//Others
	QString getIcon() const;
	QString getMouse() const;
	QString getScript() const;
	QString getWallpaper() const;
	//Window Decorations
	QString getLibrary() const;
	QString getTheme() const;
	//External
	QString getKonsole() const;

	//Maybe i don't need setters ?
	//Setters
	void setPlasma(const QString &plasma);
	void setColor(const QString &color);
	void setGtk(const QString &gtk);
	void setKvantum(const QString &kvantum);
	void setWidget(const QString &widget);
	void setIcon(const QString &icon);
	void setMouse(const QString &mouse);
	void setScript(const QString &script);
	void setWallpaper(const QString &wallpaper);
	void setLibrary(const QString &library);
	void setTheme(const QString &theme);
	void setKonsole(const QString &konsole);

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
