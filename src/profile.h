//
// Created by da-viper on 27/08/2020.
//

#ifndef PROFILE_H
#define PROFILE_H

#include <QString>
#include <QFileInfo>

//a struct may have been a better option .
class Profile
{

public:
	Profile();
	explicit Profile(const QString &profileName);

	static bool profileExists( QString profileName, QList<Profile> &profileList);
	//styles.
	void setplasma(const QString &QString);
	void setColor(const QString &QString);
	void setGtk(const QString &QString);
	void setKvantum(const QString &kvantum);
	void setWidget(const QString &kvantum);

	//icons..
	void setIcon(const QString &icon);
	void setMouse(const QString &mouse);
	void setScript(const QString &script);
	void setWallpaper(const QString &wallpaper);

	//Window Decoration
	void setDecLibrary(const QString &decorationLibrary);
	void setDecTheme(const QString &decorationTheme);

	//external
	void setKonsole(const QString &konsoleTheme);
	void setVscode(const QString &vscodeTheme);

	//getters
	//Styles
	QString getProfileName() const;
	QString getPlasma() const;
	QString getColor() const;
	QString getGtk() const;
	QString getKvantum() const;
	QString getWidget() const;

	//icons
	QString getIcon() const;
	QString getMouse() const;
	QString getScript() const;
	QString getWallpaper() const;

	//window decorations
	QString getDecLibrary() const;
	QString getDecTheme() const;

	// External
	QString getKonsole() const;
	QString getVscode() const;

private:
	QFileInfo m_path;
	QString m_profileName;
	//styles
	QString m_plasma;
	QString m_color;
	QString m_gtk;
	QString m_kvantum;
	QString m_widget;

	//other
	QString m_icon;
	QString m_mouse;
	QString m_script;
	QString m_wallpaper;

	//Window decoration
	QString m_decLibrary;
	QString m_decTheme;

	//External
	QString m_konsole;
	QString m_vscode;

};


#endif //PROFILE_H
