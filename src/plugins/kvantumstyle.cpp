#include "kvantumstyle.h"
#include "../headers/utils.h"

#include <QProcess>

void KvantumStyle::setTheme(QString kvantumStyle) {
  QStringList arguments{"--set", kvantumStyle};
  QProcess::startDetached("/usr/bin/kvantummanager", arguments);
}

QStringList KvantumStyle::getThemes() {
  const QStringList searchPaths = Utils::systemDataSearchPaths(
    QStringLiteral("Kvantum"),
    {QStringLiteral("/var/run/current-system/sw/Kvantum")});

  QStringList kvantumStyles;
  for (const QString &dirPath : searchPaths) {
    const QDir dir(dirPath);
    // Navigate theme subdirectories
    for (const QFileInfo &entry :
      dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
      // Check inside the directory for all ".kvconfig" files
      const QDir subDir(entry.absoluteFilePath());
    for (const QFileInfo &subEntry : subDir.entryInfoList(QDir::Files)) {
      if (subEntry.suffix() == QStringLiteral("kvconfig")) {
        kvantumStyles.append(subEntry.baseName());
      }
    }
      }
  }

  kvantumStyles.removeDuplicates();
  kvantumStyles.sort();
  return kvantumStyles;
}
