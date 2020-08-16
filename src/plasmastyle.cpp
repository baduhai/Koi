#include "plasmastyle.h"

/* This method is no longer used for anything
 it would be better to move some of the code concerning plasma from utils to this 
 class like get plasma style */
PlasmaStyle::PlasmaStyle()
{

}

void PlasmaStyle::setPlasmaStyle(QString plasmaStyle)
{
    KSharedConfigPtr plasmarc = KSharedConfig::openConfig(QStandardPaths::locate(QStandardPaths::GenericConfigLocation, "plasmarc"), KSharedConfig::CascadeConfig);
    KConfigGroup(plasmarc, "Theme").writeEntry("name", plasmaStyle);
}

void PlasmaStyle::setPlasmaStyleBreeze()
{
    KSharedConfigPtr plasmarc = KSharedConfig::openConfig(QStandardPaths::locate(QStandardPaths::GenericConfigLocation, "plasmarc"), KSharedConfig::CascadeConfig);
    KConfigGroup(plasmarc, "Theme").deleteGroup();
}
