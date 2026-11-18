#include "headers/utils.h"
#include "libraries/SunRise.h"

#include <QFileInfo>
#include <QStandardPaths>
#include <QThread>
#include <chrono>

Utils::Utils() {
  lightTimer.setSingleShot(true);
  darkTimer.setSingleShot(true);
  sunTimer.setSingleShot(true);

  lightTimer.setTimerType(Qt::PreciseTimer);
  darkTimer.setTimerType(Qt::PreciseTimer);
  sunTimer.setTimerType(Qt::PreciseTimer);

  connect(&lightTimer, &QTimer::timeout, this, [this]() {
    goLight();
    scheduleLight();
  });

  connect(&darkTimer, &QTimer::timeout, this, [this]() {
    goDark();
    scheduleDark();
  });

  connect(&sunTimer, &QTimer::timeout, this, [this]() {
    if (pendingSunAction == PendingSunAction::Light) {
      goLight();
    } else if (pendingSunAction == PendingSunAction::Dark) {
      goDark();
    }
    scheduleSunEvent();
  });
}

QStringList Utils::systemDataSearchPaths(const QString &subDir,
                                         const QStringList &extraPaths) {
  QStringList candidates;

  // 1. Dynamic XDG_DATA_HOME + XDG_DATA_DIRS
  // Covers Debian, Ubuntu, Fedora, openSUSE, Void, NixOS, and GNU Guix profiles
  const QStringList genericDataDirs =
      QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
  for (const QString &baseDir : genericDataDirs) {
    candidates.append(QDir(baseDir).filePath(subDir));
  }

  // 2. Explicit system fallbacks for FHS, NixOS, and GNU Guix
  const QStringList systemRoots = {
      QStringLiteral("/usr/local/share"),
      QStringLiteral("/usr/share"),
      QStringLiteral("/run/current-system/sw/share"),
      QStringLiteral("/var/run/current-system/sw/share"),
      QStringLiteral("/run/current-system/profile/share")};
  for (const QString &root : systemRoots) {
    candidates.append(QDir(root).filePath(subDir));
  }

  // 3. Caller-specific extra or legacy paths
  candidates.append(extraPaths);

  // Deduplicate by canonical path while preserving priority order
  QStringList resolved;
  QStringList seenCanonical;
  for (const QString &path : candidates) {
    const QFileInfo info(path);
    if (!info.exists() || !info.isDir()) {
      continue;
    }
    const QString canonical = info.canonicalFilePath();
    const QString key = canonical.isEmpty() ? QDir::cleanPath(path) : canonical;
    if (!seenCanonical.contains(key)) {
      seenCanonical.append(key);
      resolved.append(path);
    }
  }
  return resolved;
}

// Global settings stuff
void Utils::initialiseSettings() {
  settings = std::make_unique<QSettings>(
      QDir::homePath() + "/.config/koirc",
      QSettings::IniFormat); // Setting config path and format
}

// Miscellaneous functions
void Utils::notify(QString notifySummary, QString notifyBody,
                   int timeoutms) // Push notification through DBus
{
  if (!notifyInterface) {
    notifyInterface = new QDBusInterface(
        "org.freedesktop.Notifications", "/org/freedesktop/Notifications",
        "org.freedesktop.Notifications", QDBusConnection::sessionBus());
  }
  QString app_name = "Koi"; // What program is the notification coming from?
  uint replaces_id =
      0; // Not sure what this is. Think it has something to do with pid.
  QString app_icon; // Not actually specifying app icon, this is if you'd like
                    // to push an image along with notification.
  QString summary = notifySummary; // Title of notification.
  QString body = notifyBody;       // Notification body.
  QStringList actions;             // No idea how to use.
  QVariantMap hints; // Provide .desktop file for notification sorting in the DE
  hints.insert("desktop-entry", "Koi");
  int timeout = timeoutms; // Notification timeout, there's no way to assume
                           // system has a default timeout unfortunately.
  notifyInterface->call("Notify", app_name, replaces_id, app_icon, summary,
                        body, actions, hints, timeout);
}

void Utils::startupCheck() {
  lightTimer.stop();
  darkTimer.stop();
  sunTimer.stop();
  pendingSunAction = PendingSunAction::None;

  if (settings->value("schedule").toBool()) // Check if schedule is enabled
  {
    if (settings->value("schedule-type").toString() ==
        "time") // Check if time schedule is enabled
    {
      // Startup time check
      startupTimeCheck(); // Switch to the theme set for the current time
      // Schedule light and dark events
      scheduleLight(); // Schedule light event
      scheduleDark();  // Schedule dark event
    } else             // Auto sun switch
    {
      startupSunCheck(); // Switch to the theme set for the current sun status
      // Schedule sun event
      scheduleSunEvent(); // Schedule sun event
    }
  }
}

void Utils::startupTimeCheck() // Switch to the theme set for the current time
{
  QTime lightTime =
      QTime::fromString(settings->value("time-light").toString(), "hh:mm:ss");
  QTime darkTime =
      QTime::fromString(settings->value("time-dark").toString(), "hh:mm:ss");
  QTime now = QTime::currentTime();
  if (now < lightTime && now <= darkTime) {
    QThread::msleep(
        1000); // Needed delay, or Koi may use the wrong color scheme.
    // Check if the option current is not already set to dark
    if (settings->value("current").toString() != "Dark") {
      // If not, switch to dark mode
      goDark();
    }
  } else if (now >= lightTime && now < darkTime) {
    QThread::msleep(1000);
    // Check if the option current is not already set to light
    if (settings->value("current").toString() != "Light") {
      // If not, switch to light mode
      goLight();
    }
  } else {
    QThread::msleep(1000);
    goDark(); // Default to dark mode if the time is not set correctly
  }
}

void Utils::startupSunCheck() { // Switch to the theme set for the current sun
                                // status

  double latitude = settings->value("latitude").toDouble();
  double longitude = settings->value("longitude").toDouble();
  time_t t = time(NULL);

  SunRise sr;
  sr.calculate(latitude, longitude, t);

  if (sr.isVisible) {
    QThread::msleep(1000);
    if (settings->value("current").toString() != "Light") {
      // If not, switch to light mode
      goLight();
    }
  } else {
    QThread::msleep(1000);
    if (settings->value("current").toString() != "Dark") {
      // If not, switch to dark mode
      goDark();
    }
  }
}

// Schedule functions
// Schedule light and dark events
void Utils::scheduleLight() {
  const QString timeStr = settings->value("time-light").toString();
  QTime targetTime = QTime::fromString(timeStr, QStringLiteral("hh:mm:ss"));
  if (!targetTime.isValid()) {
    targetTime = QTime::fromString(timeStr, QStringLiteral("hh:mm"));
  }
  if (!targetTime.isValid()) {
    return;
  }

  const QDateTime now = QDateTime::currentDateTime();
  QDateTime nextRun(now.date(), targetTime);
  if (nextRun <= now.addSecs(5)) {
    nextRun = nextRun.addDays(1);
  }

  const qint64 delayMs = qMax<qint64>(1000, now.msecsTo(nextRun));
  lightTimer.start(std::chrono::milliseconds(delayMs));
}

void Utils::scheduleDark() {
  const QString timeStr = settings->value("time-dark").toString();
  QTime targetTime = QTime::fromString(timeStr, QStringLiteral("hh:mm:ss"));
  if (!targetTime.isValid()) {
    targetTime = QTime::fromString(timeStr, QStringLiteral("hh:mm"));
  }
  if (!targetTime.isValid()) {
    return;
  }

  const QDateTime now = QDateTime::currentDateTime();
  QDateTime nextRun(now.date(), targetTime);
  if (nextRun <= now.addSecs(5)) {
    nextRun = nextRun.addDays(1);
  }

  const qint64 delayMs = qMax<qint64>(1000, now.msecsTo(nextRun));
  darkTimer.start(std::chrono::milliseconds(delayMs));
}

// Schedule sun event
void Utils::scheduleSunEvent() {
  // Schedules a theme change for the next sunrise or sunfall
  const double latitude = settings->value("latitude").toDouble();
  const double longitude = settings->value("longitude").toDouble();
  const time_t t = time(nullptr);

  SunRise sr;
  sr.calculate(latitude, longitude, t);

  const QDateTime now = QDateTime::currentDateTime();
  const bool hasUpcomingRise = sr.hasRise && (sr.riseTime > sr.queryTime + 5);
  const bool hasUpcomingSet  = sr.hasSet  && (sr.setTime  > sr.queryTime + 5);

  if (!hasUpcomingRise && !hasUpcomingSet) {
    // No events found in the next SR_WINDOW/2 hours, check again later - may
    // happen in polar regions
    pendingSunAction = PendingSunAction::Recheck;
    sunTimer.start(std::chrono::hours(SR_WINDOW / 2));
  } else if (hasUpcomingRise && (!hasUpcomingSet || sr.riseTime < sr.setTime)) {
    pendingSunAction = PendingSunAction::Light;
    const QDateTime riseDt = QDateTime::fromSecsSinceEpoch(sr.riseTime);
    const qint64 delayMs = qMax<qint64>(1000, now.msecsTo(riseDt));
    sunTimer.start(std::chrono::milliseconds(delayMs));
  } else {
    pendingSunAction = PendingSunAction::Dark;
    const QDateTime setDt = QDateTime::fromSecsSinceEpoch(sr.setTime);
    const qint64 delayMs = qMax<qint64>(1000, now.msecsTo(setDt));
    sunTimer.start(std::chrono::milliseconds(delayMs));
  }
}

// Get stuff
QStringList Utils::getPlasmaStyles(void) { return plasmastyle.getThemes(); }
// Get all available Aurorae schemes
QStringList Utils::getAuroraeSchemes(void) { return aurorae.getThemes(); }
// Get all available color schemes
QStringList Utils::getColorSchemes(void) { return colorscheme.getThemes(); }
// Get all available icon themes
QStringList Utils::getIconThemes(void) { return icons.getThemes(); }
// Get all available gtk themes
QStringList Utils::getGtkThemes(void) { return gtk.getThemes(); }
// Get all available kvantum styles
QStringList Utils::getKvantumStyles(void) { return kvantumStyle.getThemes(); }
// Get all available Konsole profiles
QStringList Utils::getKonsoleProfiles(void) { return konsole.getThemes(); }

// Manage switching themes functions
void Utils::toggle() {
  const auto current =
      settings->value("current", QVariant::fromValue(Mode::Undefined))
          .value<Mode>();
  if (current == Mode::Light) {
    goDark();
  } else if (current == Mode::Dark) {
    goLight();
  }
}

void Utils::goLight() {
  goLightStyle();
  goLightAurorae();
  goLightColors();
  goLightIcons();
  goLightGtk();
  goLightKvantumStyle();
  goLightKonsole();
  goLightWall();
  goLightScript();
  restartProcess();
  if (settings->value("notify").toBool()) {
    notify("Switched to light mode!",
           "Some applications may need to be restarted for applied changes to "
           "take effect.");
  }
  settings->setValue("current", QVariant::fromValue(Mode::Light).toString());
}

void Utils::goDark() {
  goDarkStyle();
  goDarkAurorae();
  goDarkColors();
  goDarkIcons();
  goDarkGtk();
  goDarkKvantumStyle();
  goDarkKonsole();
  goDarkWall();
  goDarkScript();
  restartProcess();
  if (settings->value("notify").toBool()) {
    notify("Switched to dark mode!",
           "Some applications may need to be restarted for applied changes to "
           "take effect.");
  }
  settings->setValue("current", QVariant::fromValue(Mode::Dark).toString());
}

void Utils::goLightStyle() {
  if (settings->value("PlasmaStyle/enabled").toBool()) {
    plasmastyle.setTheme(settings->value("PlasmaStyle/light").toString());
  }
}

void Utils::goDarkStyle() {
  if (settings->value("PlasmaStyle/enabled").toBool()) {
    plasmastyle.setTheme(settings->value("PlasmaStyle/dark").toString());
  }
}

void Utils::goLightAurorae() {
  if (settings->value("Aurorae/enabled").toBool()) {
    aurorae.setTheme(settings->value("Aurorae/light").toString());
  }
}

void Utils::goDarkAurorae() {
  if (settings->value("Aurorae/enabled").toBool()) {
    aurorae.setTheme(settings->value("Aurorae/dark").toString());
  }
}

void Utils::goLightColors() {
  if (settings->value("ColorScheme/enabled").toBool()) {
    colorscheme.setTheme(settings->value("ColorScheme/light").toString());
  }
}

void Utils::goDarkColors() {
  if (settings->value("ColorScheme/enabled").toBool()) {
    colorscheme.setTheme(settings->value("ColorScheme/dark").toString());
  }
}

void Utils::goLightIcons() {
  if (settings->value("IconTheme/enabled").toBool()) {
    icons.setTheme(settings->value("IconTheme/light").toString());
  }
}

void Utils::goDarkIcons() {
  if (settings->value("IconTheme/enabled").toBool()) {
    icons.setTheme(settings->value("IconTheme/dark").toString());
  }
}

void Utils::goLightGtk() {
  if (settings->value("GTKTheme/enabled").toBool()) {
    gtk.setTheme(settings->value("GTKTheme/light").toString());
  }
}

void Utils::goDarkGtk() {
  if (settings->value("GTKTheme/enabled").toBool()) {
    gtk.setTheme(settings->value("GTKTheme/dark").toString());
  }
}

void Utils::goLightKonsole() {
  if (settings->value("KonsoleProfile/enabled").toBool()) {
    konsole.setTheme(settings->value("KonsoleProfile/light").toString());
  }
}

void Utils::goDarkKonsole() {
  if (settings->value("KonsoleProfile/enabled").toBool()) {
    konsole.setTheme(settings->value("KonsoleProfile/dark").toString());
  }
}

void Utils::goLightKvantumStyle() {
  if (settings->value("KvantumStyle/enabled").toBool()) {
    kvantumStyle.setTheme(settings->value("KvantumStyle/light").toString());
  }
}

void Utils::goDarkKvantumStyle() {
  if (settings->value("KvantumStyle/enabled").toBool()) {
    kvantumStyle.setTheme(settings->value("KvantumStyle/dark").toString());
  }
}

void Utils::goLightWall() {
  if (settings->value("Wallpaper/enabled").toBool()) {
    if (!settings->value("Wallpaper/light").isNull()) {
      wallpaper.setTheme(settings->value("Wallpaper/light").toString());
    } else {
      notify("Error setting Wallpaper",
             "Koi tried to change your wallpaper, but no wallpaper file was "
             "selected",
             0);
    }
  }
}

void Utils::goDarkWall() {
  if (settings->value("Wallpaper/enabled").toBool()) {
    if (!settings->value("Wallpaper/dark").isNull()) {
      wallpaper.setTheme(settings->value("Wallpaper/dark").toString());
    } else {
      notify("Error setting Wallpaper",
             "Koi tried to change your wallpaper, but no wallpaper file was "
             "selected",
             0);
    }
  }
}

void Utils::goLightScript() {
  if (settings->value("Script/enabled").toBool()) {
    if (!settings->value("Script/light").isNull()) {
      script.setTheme(settings->value("Script/light").toString());
    } else {
      notify("Error running script",
             "Koi tried to run your script, but no script file was selected",
             0);
    }
  }
}

void Utils::goDarkScript() {
  if (settings->value("Script/enabled").toBool()) {
    if (!settings->value("Script/dark").isNull()) {
      script.setTheme(settings->value("Script/dark").toString());
    } else {
      notify("Error running script",
             "Koi tried to run your script, but no script file was selected",
             0);
    }
  }
}

/* This updates the style of both the Plasma shell and Latte Dock if it is
 * available. It also restarts krunner to force the theme on it.
 */
void Utils::restartProcess() {
  QString kquitapp = QStandardPaths::findExecutable(QStringLiteral("kquitapp6"));
  if (kquitapp.isEmpty()) {
    kquitapp = QStringLiteral("kquitapp6");
  }

  if (settings->value("KvantumStyle/enabled").toBool()) {
    QString kstart = QStandardPaths::findExecutable(QStringLiteral("kstart"));
    if (kstart.isEmpty()) {
      kstart = QStringLiteral("kstart");
    }

    // restart plasmashell
    const QStringList plasmashell = {QStringLiteral("plasmashell")};
    QProcess::execute(kquitapp, plasmashell);
    QProcess::startDetached(kstart, plasmashell);
  }

  // stop krunner, it will be restarted when it is requested again
  QProcess::startDetached(kquitapp, {QStringLiteral("krunner")});
}
