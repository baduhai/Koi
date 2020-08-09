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
#include <QtWidgets/QStyleFactory>

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
    QStringList getCursorThemes();
    QStringList getIconThemes();
    QStringList getGtkThemes();
    QStringList getWidgetStyles();
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

    bool themeExists(QString themeName);
    void createNewTheme(const QString &pluginName, const QString &name, const QString &comment, const QString &author,
                        const QString &email, const QString &license, const QString &website);
    void writeToThemeConfigFile(const QString &pluginName, const QString &themeType);

    void dumpPlasmaLayout(const QString &pluginName);

    void dumpDefaultsConfigFile(const QString &pluginName);
private:
    Gtk gtk;
    KvantumStyle kvantumStyle;
    Wallpaper wallpaper;

    QProcess *useGlobalProcess{};

    QDBusConnection *bus{};

    QDBusInterface *notifyInterface{};
};

#endif // UTILS_H
