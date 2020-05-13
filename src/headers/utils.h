#ifndef UTILS_H
#define UTILS_H
// Headers
#include "plasmastyle.h"
#include "colorscheme.h"
#include "gtk.h"
#include "wallpaper.h"
#include "icons.h"
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
    void startupTimeCheck();
    void timerToLight();
    void timerToDark();

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
    QTimer *lightTimer = new QTimer();
    QTimer *darkTimer = new QTimer();
};

#endif // UTILS_H
