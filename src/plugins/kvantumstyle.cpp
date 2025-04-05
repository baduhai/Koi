#include "kvantumstyle.h"

#include <QProcess>

void KvantumStyle::setTheme(QString kvantumStyle) {
  QStringList arguments{"--set", kvantumStyle};
  QProcess::startDetached("/usr/bin/kvantummanager", arguments);
}

QStringList KvantumStyle::getThemes() {
  QDir kvantumStyleLocalDir(QDir::homePath() + "/.config/Kvantum");
  QDir kvantumStyleSystemDir("/usr/share/Kvantum");
  QDir kvantumStyleNixDir("/var/run/current-system/sw/Kvantum");
  QDir kvantumStyleDirs[] = {kvantumStyleLocalDir, kvantumStyleSystemDir, kvantumStyleNixDir};
  QStringList kvantumStyles;
  for (const auto& dir : kvantumStyleDirs) {
    if (dir.exists()) {
        //Navigate subdirectories
        for (const auto& entry : dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            //Check inside the directory for all ".kconfig" files
            QDir subDir(entry.absoluteFilePath());
            for (const auto& subEntry : subDir.entryInfoList(QDir::Files)) {
                if (subEntry.suffix() == "kvconfig") {
                    kvantumStyles.append(subEntry.baseName());
                }
            }
        }
    }
  }
  kvantumStyles.removeDuplicates();
  kvantumStyles.sort();
  return kvantumStyles;
}
