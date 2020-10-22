#ifndef STYLES_H
#define STYLES_H

//QT
#include <QStandardPaths>
#include <QStringList>
#include <QDir>
#include <QStyleFactory>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingCall>
#include <QProcess>
//KDE
#include <KCoreAddons>
namespace utils
{
//TODO add descriptive comments here.
QStringList getPlasmaStyles();
QStringList getColorSchemes();
QStringList getGtkThemes();
QStringList getWidgetStyles();
QStringList getKvantumStyles();

/*this won't be accessed outside*/
namespace noUse
{
void setGtk(const QString &gtkTheme);

/***enables the kvantum theme by calling the command
    kvantummanger with the argument "--set"
*/
void setKvantumStyle(QString KvantumStyle);
}
}


#endif //STYLES_H
