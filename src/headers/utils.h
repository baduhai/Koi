#ifndef UTILS_H
#define UTILS_H
// Headers
#include "gtk.h"
#include "wallpaper.h"
// Qt libs
#include <QStringList>
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

    QStringList getIconThemes(void);

    QStringList getCursorThemes(void);

    QStringList getGtkThemes(void);

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
    Gtk gtk;
    Wallpaper wallpaper;
    QTime now;
    QTime lightTime;
    QTime darkTime;
    QTimer *timer;
};

#endif // UTILS_H
