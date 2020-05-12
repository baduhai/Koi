#ifndef UTILS_H
#define UTILS_H
// Headers
#include "plasmastyle.h"
#include "colorscheme.h"
#include "gtk.h"
#include "wallpaper.h"
#include "icons.h"
// Qt libs
#include <QDir>
#include <QProcess>
#include <QSettings>
#include <QTime>
#include <QTimer>

class Utils
{
public:
    Utils();

    QSettings *settings;

    void initialiseSettings();

    QStringList getPlasmaStyles(void);

    QStringList getColorSchemes(void);

    QStringList getColorSchemesPath(void);

    QStringList getIconThemes(void);

    QStringList getCursorThemes(void);

    QStringList getGtkThemes(void);

    void notify(QString notifySummary, QString notifyBody, int timeoutms);

    void timeLoopLight();

    void timeLoopDark();

    void goLight();

    void goDark();

    void goLightStyle();

    void goDarkStyle();

    void goLightColors();

    void goDarkColors();

    void goLightIcons();

    void goDarkIcons();

    void goLightGtk();

    void goDarkGtk();

    void goLightWall();

    void goDarkWall();

private:
    PlasmaStyle plasmastyle;
    ColorScheme colorscheme;
    Icons icons;
    Gtk gtk;
    Wallpaper wallpaper;
    QDBusConnection *bus;
    QDBusInterface *notifyInterface;
    QTime now;
    QTime lightTime;
    QTime darkTime;
    QTimer *timer;
};

#endif // UTILS_H
