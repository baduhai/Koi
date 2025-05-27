#pragma once

#include <memory>

// Qt libraries
#include <QDateTime>
#include <QDir>
#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QTimer>
#include <QtGlobal>

// Headers
#include "src/plugins/colorscheme.h"
#include "src/plugins/gtk.h"
#include "src/plugins/icons.h"
#include "src/plugins/kvantumstyle.h"
#include "src/plugins/plasmastyle.h"
#include "src/plugins/script.h"
#include "src/plugins/wallpaper.h"
// Other libraries
#include "../libraries/Scheduler.h"

class Utils : public QObject {
  Q_OBJECT

public:
  enum Mode {
    Undefined = 0,
    Dark = 1,
    Light = 2,
  };
  Q_ENUM(Mode)

  Utils();

  std::unique_ptr<QSettings> settings;
  std::unique_ptr<Bosma::Scheduler> scheduler;

  void initialiseSettings();

  QStringList getPlasmaStyles(void);
  QStringList getColorSchemes(void);
  QStringList getIconThemes(void);
  QStringList getCursorThemes(void);
  QStringList getGtkThemes(void);
  QStringList getKvantumStyles(void);

  void notify(QString notifySummary = "", QString notifyBody = "",
              int timeoutms = 5000);
  void startupCheck();
  void startupTimeCheck();
  void startupSunCheck();
  void scheduleLight(Bosma::Scheduler& s);
  void scheduleDark(Bosma::Scheduler& s);
  void scheduleSunEvent(Bosma::Scheduler& s);

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

  QDBusInterface* notifyInterface = nullptr;
};
