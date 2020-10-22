
#include "utils.h"

namespace utils
{
void notify(const QString &title, const QString &body, int timeoutms) // Push notification through DBus
{
    QDBusMessage m = QDBusMessage::createMethodCall("org.freedesktop.Notifications",
                                                    "/org/freedesktop/Notifications",
                                                    "org.freedesktop.Notifications",
                                                    "Notify");
    m.setArguments({QCoreApplication::applicationName(), //Name
                    uint(0), //Pid
                    QString(QDir::rootPath()
                                + QStringLiteral("usr/share/icons/hicolor/scalable/apps/koi.svg")), //iconPath set in the src/cmakeLists.txt
                    title, //Notification Title
                    body, //Notification Body
                    QStringList(), // Action
                    QVariantMap(), //Hints
                    timeoutms // Notification time length
                   });
    QDBusConnection::sessionBus().send(m);
}

QString startupTimeCheck() // get the nearest earlier favourite theme.
{
    QSettings settings;
    settings.beginGroup("Favourites");
    auto favList = settings.allKeys();
    QTime currentTime = QTime::currentTime();
    int nearest = 0;
    QString nearestName;

    for (const auto &favourite: favList) {
        QString timeStr = settings.value(favourite).toString();
        if (timeStr.isEmpty() || timeStr.isNull()) {
            continue;
        }
        QTime favTime(QTime::fromString(timeStr));
        int timeSec = currentTime.secsTo(favTime);
        if (timeSec > 0) {
            timeSec -= 86400; //seconds in a day;
        }
        if (nearest == 0) {
            nearestName = favourite;
            nearest = timeSec;
            continue;
        }
        else if (nearest < 0 && timeSec > 0) {
            continue;
        }
        else if (nearest > timeSec) {
            continue;
        }
        else {
            nearest = timeSec;
            nearestName = favourite;
        }
    }
    return nearestName;
}

// Use to switch to a different theme profile
void go(Profile *profile)
{
    QSettings s;
    //Kvantum
    QString widget(profile->getWidget());
    if (widget == "kvantum" || widget == "kvantum-dark") {
        noUse::setKvantumStyle(profile->getKvantum());
    }
    noUse::useGlobalTheme(profile->pluginName());
    //this need to be above useGlobal as breeze changes the gtk theme
    // and causes theme mismatch
    noUse::setGtk(profile->getGtk());

    //script
    if (profile->getScriptEnabled()) {
        if (!QProcess::startDetached("/bin/sh", {profile->getScript()})) {
            qDebug() << "Failed to run " + profile->name() + " script";
        }
    }

    //wallpaper
    if (profile->getWallEnabled()) {
        noUse::setWallpaper(profile->getWallpaper());
    }
    else {
        notify("Error setting Wallpaper",
               "Koi tried to change your " + profile->name() + " wallpaper, but no wallpaper fie was selected",
               5000);
    }

    //External
    if (profile->getKonsoleEnabled()) {
        noUse::setKonsoleTheme(profile->getKonsole());
    }

    //update colours;
    auto *krdbProcess = new QProcess();
    krdbProcess->setProgram(QStringLiteral("krdb"));
    QObject::connect(krdbProcess,
                     qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
                     krdbProcess,
                     &QProcess::deleteLater);
    krdbProcess->start();
    if (s.value("notify").toBool()) {
        notify("Switched to " + profile->name() + " mode!",
               "Some applications may need to be restarted for applied changes to take effect.", 4000);
    }
}

namespace noUse
{
// Manage switching plasma themes
void useGlobalTheme(const QString &pluginName)
{
    auto *useGlob = new QProcess();
    QString command = QStringLiteral("lookandfeeltool");
    QStringList arguments = {QStringLiteral("-a"), pluginName};
    /** UPDATE it is a bug in QT version 5.15.1
     * i don't know why if i use the below command i get
     * SIGTRAP error when debugging if above any of the functions that calls Dbus
     * in the Utils::go() function above.
     */
    QObject::connect(useGlob,
                     qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
                     useGlob,
                     &QProcess::deleteLater);
    useGlob->start(command, arguments);
}
}
}


