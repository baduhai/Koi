#include "plasmastyle.h"

/* This method is no longer used for anything
 it would be better to move some of the code concerning plasma from utils to this 
 class like get plasma style */
PlasmaStyle::PlasmaStyle()
{

}

QStringList PlasmaStyle::getPlasmaStyles() // Get all available plasma styles
{
    QDir stylesLocalDir(QDir::homePath() + "/.local/share/plasma/desktoptheme");
    QDir stylesSystemDir("/usr/share/plasma/desktoptheme");
    QStringList plasmaStyles;
    if (stylesLocalDir.exists())
    {
        plasmaStyles.append(stylesLocalDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    }
    if (stylesSystemDir.exists())
    {
        plasmaStyles.append(stylesSystemDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    }
    plasmaStyles.removeDuplicates();
    plasmaStyles.append("breeze");
    return plasmaStyles;
}

