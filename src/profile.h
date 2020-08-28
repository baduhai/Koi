//
// Created by da-viper on 27/08/2020.
//

#ifndef PROFILE_H
#define PROFILE_H

#include <QString>
#include <QFileInfo>
#include <utility>

class Profile
{

public:
	Profile ();
	explicit Profile ( QString profileName);
	//styles.
	void setplasma(const QString& QString);
	void setColor(const QString& QString);
	void setGtk(const QString& QString);
	void setKvantum(const QString& QString);
	void setWidget(const QString& QString);

	//icons..
	void setIcon(const QString& QString);

	QString getPlasma();

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
