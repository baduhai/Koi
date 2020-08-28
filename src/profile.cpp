//
// Created by da-viper on 27/08/2020.
//

#include "profile.h"

#include <utility>

Profile::Profile()
{
}
Profile::Profile(QString profileName)
{
	m_profileName = QStringLiteral("Koi-") + profileName ;
}
void Profile::setplasma(const QString& plasmastyle)
{
	m_plasma = plasmastyle;
}

void Profile::setColor(const QString& colorScheme)
{
	m_color = colorScheme;
}

void Profile::setGtk(const QString& gtkScheme)
{
	m_gtk = gtkScheme;
}

void Profile::setIcon(const QString& icon)
{
	m_icon = icon;
}

QString Profile::getPlasma()
{
	return m_plasma;
}