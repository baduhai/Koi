//
// Created by da-viper on 16/10/2020.
//

#include "styles.h"

namespace utils
{


//Get all Needed this for a profile.
//PlasmaStyle
QStringList getPlasmaStyles() // Get all available plasma styles
{
    QStringList plasmaStyles;
    QFileInfoList plasmaDirs;
    QStringList plasmaDirList(QStandardPaths::locateAll(QStandardPaths::GenericDataLocation,
                                                        "plasma/desktoptheme",
                                                        QStandardPaths::LocateDirectory));
    for (const auto &dir: plasmaDirList) {
        plasmaDirs.append(QDir(dir).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden));
    }

    for (const auto &path : qAsConst(plasmaDirs)) {
        QFileInfo file(path.absoluteFilePath() + QStringLiteral("/metadata.desktop"));
        if (file.exists()) {
            //TODO: show the pluginName instead in the view ;
            plasmaStyles.append(path.baseName());
        }
    }

    plasmaStyles.removeAll(QStringLiteral("default"));//should not be included
    plasmaStyles.removeDuplicates();
    plasmaStyles.sort();
    return plasmaStyles;
}

//Color Schemes
//TODO use standard paths
QStringList getColorSchemes() // Get all available color schemes
{
    QStringList colorNames;
    QFileInfoList cList;
    const QStringList colorDir = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation,
                                                           QStringLiteral("color-schemes"),
                                                           QStandardPaths::LocateDirectory);
    for (const auto &dir : colorDir) {
        QDir s(dir);
        s.setFilter(QDir::Files);
        cList.append(s.entryInfoList({QStringLiteral("*.colors")}));
    }
    for (const auto &path: cList) {
        colorNames.append(path.baseName());
    }
    colorNames.sort();
    return colorNames;
}

//GTK
QStringList getGtkThemes() // Get all available gtk themes
{
    QStringList gtkThemes;
    QString gtkLocalDir(QDir::homePath() + QStringLiteral("/.themes"));
    //all the possible path of gtkthemes.
    QStringList gtkDirList
        (QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, "themes", QStandardPaths::LocateDirectory));
    gtkDirList.append(gtkLocalDir);
    for (const auto &path: gtkDirList) {
        QDir gtkDir(path);
        if (!gtkDir.exists()) {
            continue;
        }
        auto themeList(gtkDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot));

        for (const auto &tName: themeList) {
            QDir themeDir(tName.absoluteFilePath());
            if (themeDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).contains(QStringLiteral("gtk-3.0"))) {
                gtkThemes.append(themeDir.dirName());
            }
        }
    }
    gtkThemes.removeAll(QStringLiteral("Default"));
    gtkThemes.removeAll(QStringLiteral("Breeze-Dark"));//same as Breeze
    gtkThemes.removeDuplicates();
    gtkThemes.sort();
    return gtkThemes;
}

//widget styles
QStringList getWidgetStyles()
{
    //this literally took me 2 hrs to find.
    QStringList widgetStyles = QStyleFactory::keys();
    widgetStyles.sort();
    return widgetStyles;
}

//Kvantum
QStringList getKvantumStyles() // Get all available kvantum styles
{
    /***FixMe: there are other kvantum dirs to check for here is the list
     * use the config file for parsing
     * /usr/share/Kvantum
     * /usr/share/themes/Kvantum
     * ~/.config/Kvantum/$THEME_NAME/
     * ~/.themes/$THEME_NAME/Kvantum/
     * ~/.local/share/themes/$THEME_NAME/Kvantum/
     */

    QDir kvantumStyleLocalDir(QDir::homePath() + "/.config/Kvantum");
    QDir kvantumStyleSystemDir(QDir::rootPath() + "usr/share/Kvantum");
    QStringList kvantumStyles;
    if (kvantumStyleLocalDir.exists()) {
        kvantumStyles.append(kvantumStyleLocalDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    }
    if (kvantumStyleSystemDir.exists()) {
        kvantumStyles.append(kvantumStyleSystemDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    }
    kvantumStyles.removeDuplicates();
    kvantumStyles.sort();
    return kvantumStyles;
}

namespace noUse // No to use outside the name space
{

void setGtk(const QString &gtkTheme)
{
    QString method;
    if (KCoreAddons::version() < QT_VERSION_CHECK(5, 75, 0)) {
        method = QStringLiteral("setGtk3Theme");
        QDBusConnection::sessionBus().asyncCall(QDBusMessage::createMethodCall("org.kde.GtkConfig",
                                                                               "/GtkConfig",
                                                                               "org.kde.GtkConfig",
                                                                               "setGtk2Theme"));
    }
    if (KCoreAddons::version() >= QT_VERSION_CHECK(5, 75, 0)) {
        method = QStringLiteral("setGtkTheme");
    }

    auto message = QDBusMessage::createMethodCall("org.kde.GtkConfig",
                                                  "/GtkConfig",
                                                  "org.kde.GtkConfig",
                                                  method);
    message.setArguments({gtkTheme});
    QDBusConnection::sessionBus().asyncCall(message);
}

void setKvantumStyle(QString kvantumStyle)
{
    auto kvProcess = new QProcess;
    QString program = "kvantummanager";
    QStringList arguments{"--set", std::move(kvantumStyle)};
    QObject::connect(kvProcess,
                     qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
                     kvProcess,
                     &QProcess::deleteLater);
    kvProcess->start(program, arguments);
}
}
}