//

#include "profile.h"


Profile::Profile()
{
}


Profile::Profile(const QString &profileName)
{
	m_profileName = QStringLiteral("Koi-") + profileName;
}

bool Profile::profileExists(QString profileName, QList<Profile> &profileList)
{
	for( const auto  &pf : profileList){
		if( QString::compare(profileName , pf.getProfileName(), Qt::CaseInsensitive) == 0 ) {
			return true;
		}
	}
	return false;
}
//Setters
//Styles
void Profile::setplasma(const QString &plasmastyle)
{ m_plasma = plasmastyle; }

void Profile::setColor(const QString &colorScheme)
{ m_color = colorScheme; }

void Profile::setGtk(const QString &gtkScheme)
{ m_gtk = gtkScheme; }

void Profile::setKvantum(const QString &kvantum)
{ m_kvantum = kvantum; }

void Profile::setWidget(const QString &widget)
{ m_widget = widget; }

//icons
void Profile::setIcon(const QString &icon)
{ m_icon = icon; }

void Profile::setMouse(const QString &mouse)
{ m_mouse = mouse; }

void Profile::setScript(const QString &script)
{ m_script = script; }

void Profile::setWallpaper(const QString &wallpaper)
{ m_wallpaper = wallpaper; }

void Profile::setDecLibrary(const QString &decorationLibrary)
{ m_decLibrary = decorationLibrary; }

void Profile::setDecTheme(const QString &decorationTheme)
{ m_decTheme = decorationTheme; }

void Profile::setKonsole(const QString &konsoleTheme)
{ m_konsole = konsoleTheme; }

void Profile::setVscode(const QString &vscodeTheme)
{ m_vscode = vscodeTheme; }

//getters
//Styles
QString Profile::getProfileName() const
{ return m_profileName; }

QString Profile::getPlasma() const
{ return m_plasma; }

QString Profile::getColor() const
{ return m_color; }

QString Profile::getGtk() const
{ return m_gtk; }

QString Profile::getKvantum() const
{ return m_kvantum; }

QString Profile::getWidget() const
{ return m_widget; }

//Icons
QString Profile::getIcon() const
{ return m_icon; }

QString Profile::getMouse() const
{ return m_mouse; }

QString Profile::getScript() const
{ return m_script; }

QString Profile::getWallpaper() const
{ return m_wallpaper; }

//window decoration
QString Profile::getDecLibrary() const
{ return m_decLibrary; }

QString Profile::getDecTheme() const
{ return m_decTheme; }

//External
QString Profile::getKonsole() const
{ return m_konsole; }

QString Profile::getVscode() const
{ return m_vscode; }

