#pragma once

#include <memory>

// Qt libraries
#include <QDateTime>
#include <QDir>
#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QtGlobal>

// Headers
#include "src/plugins/aurorae.h"
#include "src/plugins/colorscheme.h"
#include "src/plugins/gtk.h"
#include "src/plugins/icons.h"
#include "src/plugins/konsole.h"
#include "src/plugins/kvantumstyle.h"
#include "src/plugins/plasmastyle.h"
#include "src/plugins/script.h"
#include "src/plugins/wallpaper.h"

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

  static QStringList systemDataSearchPaths(const QString &subDir,
                                           const QStringList &extraPaths = {});

  std::unique_ptr<QSettings> settings;

  void initialiseSettings();

  QStringList getPlasmaStyles(void);
  QStringList getAuroraeSchemes(void);
  QStringList getColorSchemes(void);
  QStringList getIconThemes(void);
  QStringList getCursorThemes(void);
  QStringList getGtkThemes(void);
  QStringList getKvantumStyles(void);
  QStringList getKonsoleProfiles(void);

  void notify(QString notifySummary = "", QString notifyBody = "",
              int timeoutms = 5000);
  void startupCheck();
  void startupTimeCheck();
  void startupSunCheck();
  void scheduleLight();
  void scheduleDark();
  void scheduleSunEvent();

  void toggle();
  void goLight();
  void goDark();
  void goLightStyle();
  void goDarkStyle();
  void goLightAurorae();
  void goDarkAurorae();
  void goLightColors();
  void goDarkColors();
  void goLightIcons();
  void goDarkIcons();
  void goLightGtk();
  void goDarkGtk();
  void goLightKonsole();
  void goDarkKonsole();
  void goLightKvantumStyle();
  void goDarkKvantumStyle();
  void goLightWall();
  void goDarkWall();
  void goLightScript();
  void goDarkScript();
  void restartProcess();

private:
  enum class PendingSunAction {
    None,
    Recheck,
    Light,
    Dark,
  };

  Aurorae aurorae;
  PlasmaStyle plasmastyle;
  ColorScheme colorscheme;
  Icons icons;
  Gtk gtk;
  Wallpaper wallpaper;
  KvantumStyle kvantumStyle;
  Script script;
  Konsole konsole;

  QTimer lightTimer;
  QTimer darkTimer;
  QTimer sunTimer;
  PendingSunAction pendingSunAction = PendingSunAction::None;

  QDBusInterface *notifyInterface = nullptr;
};