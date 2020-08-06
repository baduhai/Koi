#ifndef UTILS_H
#define UTILS_H
// Headers
#include "plasmastyle.h"
#include "colorscheme.h"
#include "gtk.h"
#include "wallpaper.h"
#include "icons.h"
#include "kvantumstyle.h"

// Qt libs
#include <QtGlobal>
#include <QObject>
#include <QDir>
#include <QProcess>
#include <QSettings>
#include <QDateTime>
#include <QTimer>
#include <QTest>

class Utils : public QObject
{
    Q_OBJECT

public:
    Utils();

    QSettings *settings{};
    void initialiseSettings();

    QStringList getPlasmaStyles();
    QStringList getColorSchemes();
    QStringList getColorSchemesPath();
    QStringList getIconThemes();
    QStringList getGtkThemes();
    QStringList getKvantumStyles();

    void notify(QString notifySummary = "", QString notifyBody = "", int timeoutms = 5000);
    void startupTimeCheck();
    //this would be used for global theme
    void useGlobalTheme(QString type);
    void goLight();
    void goDark();
    void goLightGtk();
    void goDarkGtk();
    void goLightKvantumStyle();
    void goDarkKvantumStyle();
    void goLightWall();
    void goDarkWall();

private:
    Gtk gtk;
    KvantumStyle kvantumStyle;
    Wallpaper wallpaper;
    QProcess *useGlobalProcess{};
    QDBusConnection *bus{};
    QDBusInterface *notifyInterface{};
};

#endif // UTILS_H
