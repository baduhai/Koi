#ifndef UTILS_H
#define UTILS_H
// Headers
#include "plasmastyle.h"
#include "colorscheme.h"
#include "gtk.h"
#include "wallpaper.h"
#include "icons.h"
#include "kvantumstyle.h"
#include "script.h"

// Qt libraries
#include <QtGlobal>
#include <QObject>
#include <QDir>
#include <QProcess>
#include <QSettings>
#include <QDateTime>
#include <QTimer>
#include <QTest>
#include <QProcess>

// Misc libraries
#include "../libraries/SunRise.h"

class Utils : public QObject
{
    Q_OBJECT

public:
    enum Mode {
        Undefined = 0,
        Dark = 1,
        Light = 2,
    };
    Q_ENUM(Mode)

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
    void startupSunCheck();

    void toggle();
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
    void goLightKvantumStyle();
    void goDarkKvantumStyle();
    void goLightWall();
    void goDarkWall();
    void goLightScript();
    void goDarkScript();
    void restartProcess();

private:
    PlasmaStyle plasmastyle;
    ColorScheme colorscheme;
    Icons icons;
    Gtk gtk;
    Wallpaper wallpaper;
    KvantumStyle kvantumStyle;
    Script script;

    QDBusConnection *bus;
    QDBusInterface *notifyInterface;
    QProcess *plasmaDesktopProcess;
    QProcess *latteProcess;
    QProcess *kquitappProcess;
    QProcess *kstartProcess;
    QProcess *krunnerProcess;
};

#endif // UTILS_H
