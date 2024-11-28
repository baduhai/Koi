#pragma once

// Qt libraries
#include <QDateTime>
#include <QDir>
#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QTest>
#include <QTimer>
#include <QtGlobal>

// Headers
#include "plugins/colorscheme.h"
#include "plugins/gtk.h"
#include "plugins/icons.h"
#include "plugins/kvantumstyle.h"
#include "plugins/plasmastyle.h"
#include "plugins/script.h"
#include "plugins/wallpaper.h"

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

  QSettings *settings;
  void initialiseSettings();

  QStringList getPlasmaStyles(void);
  QStringList getColorSchemes(void);
  QStringList getIconThemes(void);
  QStringList getCursorThemes(void);
  QStringList getGtkThemes(void);
  QStringList getKvantumStyles(void);

  void notify(QString notifySummary = "", QString notifyBody = "",
              int timeoutms = 5000);
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
