
#include "utils.h"

namespace utils
{
void notify(QString notifySummary, QString notifyBody, int timeoutms) // Push notification through DBus
{
    QDBusInterface notifyInterface("org.freedesktop.Notifications",
                                   "/org/freedesktop/Notifications",
                                   "org.freedesktop.Notifications",
                                   QDBusConnection::sessionBus());
    QString app_name = "Koi";        // What program is the notification coming from?
    uint replaces_id = 0;            // Not sure what this is. Think it has something to do with pid.
    QString
        app_icon;                // Not actually specifying app icon, this is if you'd like to push an image alog with notification.
    QString summary = std::move(notifySummary); // Title of notification.
    QString body = std::move(notifyBody);       // Notification body.
    QStringList actions;             // No idea how to use.
    QVariantMap hints;               // No idea how to use.
    int timeout =
        timeoutms;         // Notification timeout, there's no way to assume system has a default timeout unfortunately.
    notifyInterface.call("Notify", app_name, replaces_id, app_icon, summary, body, actions, hints, timeout);
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
               "Some applications may need to be restarted for applied changes to take effect.");
    }
}

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


