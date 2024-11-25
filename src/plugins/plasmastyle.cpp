#include "plasmastyle.h"

void PlasmaStyle::setTheme(QString plasmaStyle) {
  process = new QProcess;
  QString style = "/usr/bin/plasma-apply-desktoptheme";
  QStringList styleArgs = {plasmaStyle};
  process->start(style, styleArgs);
  process->waitForFinished();
  process->close();
}
