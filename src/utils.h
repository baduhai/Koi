#ifndef UTILS_H
#define UTILS_H
// Headers
#include "plasmastyle.h"
#include "colorscheme.h"
#include "gtk.h"
#include "wallpaper.h"
#include "icons.h"
#include "kvantumstyle.h"
#include "useglobal.h"

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

    QSettings *settings;
    void initialiseSettings();

    QStringList getPlasmaStyles(void);
    QStringList getColorSchemes(void);
    QStringList getColorSchemesPath(void);
    QStringList getIconThemes(void);
    QStringList getCursorThemes(void);
    QStringList getGtkThemes(void);
    QStringList getKvantumStyles(void);

    void notify(QString notifySummary = "", QString notifyBody = "", int timeoutms = 5000);
    void startupTimeCheck();
    //this would be used for global theme 
    void useTheme(QString type);
    void goLight();
    void goDark();
    void goLightGtk();
    void goDarkGtk();
    void goLightWall();
    void goDarkWall();

private:
    Gtk gtk;
    Wallpaper wallpaper;
    UseGlobal globaltheme;

    QDBusConnection *bus;
    QDBusInterface *notifyInterface;
};

#endif // UTILS_H
