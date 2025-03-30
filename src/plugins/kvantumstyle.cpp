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