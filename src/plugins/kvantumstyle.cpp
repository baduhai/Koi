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
  QStringList kvantumStyles;
  if (kvantumStyleLocalDir.exists()) {
    kvantumStyles.append(
        kvantumStyleLocalDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
  }
  if (kvantumStyleSystemDir.exists()) {
    kvantumStyles.append(
        kvantumStyleSystemDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
  }
  if (kvantumStyleNixDir.exists()) {
    kvantumStyles.append(
        kvantumStyleNixDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
  }
  kvantumStyles.removeDuplicates();
  kvantumStyles.sort();
  return kvantumStyles;
}