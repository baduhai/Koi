#include "plasmastyle.h"
#include "../headers/utils.h"

#include <QProcess>

void PlasmaStyle::setTheme(QString plasmaStyle) {
  QStringList styleArgs = {plasmaStyle};
  QProcess::execute("/usr/bin/plasma-apply-desktoptheme", styleArgs);
}

QStringList PlasmaStyle::getThemes(void) {
  const QStringList searchPaths =
  Utils::systemDataSearchPaths(QStringLiteral("plasma/desktoptheme"));

  QStringList plasmaStyles;
  for (const QString &dirPath : searchPaths) {
    const QDir dir(dirPath);
    plasmaStyles.append(dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
  }

  plasmaStyles.removeDuplicates();
  plasmaStyles.sort();
  return plasmaStyles;
}
