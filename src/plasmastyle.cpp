#include "plasmastyle.h"

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
