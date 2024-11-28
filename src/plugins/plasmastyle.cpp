#include "plasmastyle.h"

void PlasmaStyle::setTheme(QString plasmaStyle) {
  process = new QProcess;
  QString style = "/usr/bin/plasma-apply-desktoptheme";
  QStringList styleArgs = {plasmaStyle};
  process->start(style, styleArgs);
  process->waitForFinished();
  process->close();
}

QStringList PlasmaStyle::getThemes(void) {
  QStringList plasmaStyles;
  QDir stylesLocalDir(QDir::homePath() + "/.local/share/plasma/desktoptheme");
  QDir stylesSystemDir("/usr/share/plasma/desktoptheme");
  QDir stylesNixDir("/var/run/current-system/sw/share/plasma/desktoptheme");
  if (stylesLocalDir.exists()) {
    plasmaStyles = plasmaStyles +
                   stylesLocalDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    ;
  }
  if (stylesSystemDir.exists()) {
    plasmaStyles = plasmaStyles +
                   stylesSystemDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    ;
  };
  if (stylesNixDir.exists()) {
    plasmaStyles = plasmaStyles +
                   stylesNixDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    ;
  }
  plasmaStyles.removeDuplicates();
  plasmaStyles.sort();
  return plasmaStyles;
}