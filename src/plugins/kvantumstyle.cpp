#include "kvantumstyle.h"

void KvantumStyle::setTheme(QString kvantumStyle) {
  process = new QProcess;
  QString program = "/usr/bin/kvantummanager";
  QStringList arguments{"--set", kvantumStyle};
  process->start(program, arguments);
}
