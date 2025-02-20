#include "kvantumstyle.h"

void KvantumStyle::setTheme(QString kvantumStyle) {
  process = new QProcess;
  QString program = "/usr/bin/kvantummanager";
  QStringList arguments{"--set", kvantumStyle};
  process->start(program, arguments);
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
