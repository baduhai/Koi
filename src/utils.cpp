#include "headers/utils.h"
#include "libraries/SunRise.h"

#include <QThread>

Utils::Utils() {}

// Global settings stuff
void Utils::initialiseSettings() {
  settings =
      std::make_unique<QSettings>(QDir::homePath() + "/.config/koirc",
                    QSettings::IniFormat); // Setting config path and format
}

// Miscelaneous functions
void Utils::notify(QString notifySummary, QString notifyBody,
                   int timeoutms) // Push notification through DBus
{
  if (!notifyInterface)
  {
    notifyInterface = new QDBusInterface("org.freedesktop.Notifications",
                                       "/org/freedesktop/Notifications",
                                       "org.freedesktop.Notifications", QDBusConnection::sessionBus());
  }
  QString app_name = "Koi"; // What program is the notification coming from?
  uint replaces_id =
      0; // Not sure what this is. Think it has something to do with pid.
  QString app_icon; // Not actually specifying app icon, this is if you'd like
                    // to push an image alog with notification.
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
void Utils::startupCheck()
{
  if(settings->value("schedule").toBool()) // Check if schedule is enabled
  {
    //Init scheduler object
    scheduler = std::make_unique<Bosma::Scheduler>(2);
    if(settings->value("schedule-type").toString() == "time") // Check if time schedule is enabled
    {
      //Startup time check
      startupTimeCheck(); // Switch to the theme set for the current time
      // Schedule light and dark events
      scheduleLight(*scheduler); // Schedule light event
      scheduleDark(*scheduler); // Schedule dark event
    }
    else // Auto sun switch
    {
      startupSunCheck(); // Switch to the theme set for the current sun status
      //Schedule sun event
      scheduleSunEvent(*scheduler); // Schedule sun event
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
    QThread::msleep(1000); // Needed delay, or Koi may use the wrong color scheme.
    // Check if the option current is not already set to dark
    if (settings->value("current").toString() != "Dark") {
      // If not, switch to dark mode
      goDark();
    }
  }
  else if (now >= lightTime && now < darkTime) {
    QThread::msleep(1000);
    //Check if the option current is not already set to light
    if (settings->value("current").toString() != "Light") {
      // If not, switch to light mode
      goLight();
    }
  }
  else {
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
void Utils::scheduleLight(Bosma::Scheduler& s) {
  int lightCronMin =
      QTime::fromString(settings->value("time-light").toString()).minute();
  int lightCronHr =
      QTime::fromString(settings->value("time-light").toString()).hour();
  if (lightCronMin <= 0) {
    lightCronMin = 0;
  }
  if (lightCronHr <= 0) {
    lightCronHr = 0;
  }
  std::string lightCron = std::to_string(lightCronMin) + " " +
                          std::to_string(lightCronHr) + " * * *";
  s.cron(lightCron, [this]() { goLight(); });
}

void Utils::scheduleDark(Bosma::Scheduler& s) {
  int darkCronMin =
      QTime::fromString(settings->value("time-dark").toString()).minute();
  int darkCronHr =
      QTime::fromString(settings->value("time-dark").toString()).hour();
  if (darkCronMin <= 0) {
    darkCronMin = 0;
  }
  if (darkCronHr <= 0) {
    darkCronHr = 0;
  }
  std::string darkCron =
      std::to_string(darkCronMin) + " " + std::to_string(darkCronHr) + " * * *";
  s.cron(darkCron, [this]() { goDark(); });
}
// Schedule sun event
void Utils::scheduleSunEvent(Bosma::Scheduler& s) {
  // Schedules a theme change for the next sunrise or sunfall
  double latitude = settings->value("latitude").toDouble();
  double longitude = settings->value("longitude").toDouble();

  time_t t = time(NULL);

  SunRise sr;
  sr.calculate(latitude, longitude, t);

  char buffer[20];
  struct tm *timeinfo;

  if ((!sr.hasRise || (sr.hasRise && sr.riseTime < sr.queryTime)) &&
      (!sr.hasSet || (sr.hasSet && sr.setTime < sr.queryTime))) {
    // No events found in the next SR_WINDOW/2 hours, check again later - may
    // happen in polar regions
    s.in(std::chrono::hours(SR_WINDOW / 2), [this, &s]() { scheduleSunEvent(s); });
  } else if (sr.hasRise && sr.riseTime > sr.queryTime) {
    timeinfo = localtime(&sr.riseTime);
    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", timeinfo);
    std::string sunEventCron = buffer;
    s.at(sunEventCron, [this, &s]() {
      goLight();
      scheduleSunEvent(s);
    });
  } else if (sr.hasSet && sr.setTime > sr.queryTime) {
    timeinfo = localtime(&sr.setTime);
    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", timeinfo);
    std::string sunEventCron = buffer;
    s.at(sunEventCron, [this, &s]() {
      goDark();
      scheduleSunEvent(s);
    });
  }
}

// Get stuff
QStringList Utils::getPlasmaStyles(void) { return plasmastyle.getThemes(); }
// Get all available color schemes
QStringList Utils::getColorSchemes(void) { return colorscheme.getThemes(); }
// Get all available icon themes
QStringList Utils::getIconThemes(void) { return icons.getThemes(); }
// Get all available gtk themes
QStringList Utils::getGtkThemes(void) { return gtk.getThemes(); }
// Get all available kvantum styles
QStringList Utils::getKvantumStyles(void) { return kvantumStyle.getThemes(); }
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
  goLightColors();
  goLightIcons();
  goLightGtk();
  goLightKvantumStyle();
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
  goDarkColors();
  goDarkIcons();
  goDarkGtk();
  goDarkKvantumStyle();
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

/* this updates the style of both the plasma shell and latte  dock if it is
 * available It also restart krunner to force the theme on it
 */
void Utils::restartProcess() {
  if (settings->value("KvantumStyle/enabled").toBool()) {

    // restart plasmashell
    QStringList plasmashell = {"plasmashell"};
    QProcess::execute("/usr/bin/kquitapp6", plasmashell);
    QProcess::startDetached("/usr/bin/kstart", plasmashell);
  }

  // stop krunner, it will be restarted when it is requested again
  QProcess::startDetached("/usr/bin/kquitapp6", {"krunner"});
}
