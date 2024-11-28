#include "colorscheme.h"

void ColorScheme::setTheme(QString themeName) {
  process = new QProcess;

  QString locateProgram = "whereis";
  QStringList programToLocate = {"plasma-apply-colorscheme"};

  process->start(locateProgram, programToLocate);
  process->waitForFinished();

  QString program(process->readAllStandardOutput());
  program.replace("plasma-apply-colorscheme: ", "");
  program.replace("\n", "");

  QStringList arguments{themeName};
  process->start(program, arguments);
}

QStringList ColorScheme::getThemes() {
  QStringList colorSchemesNames;
  QDir colorsLocalDir(QDir::homePath() + "/.local/share/color-schemes");
  QDir colorsSystemDir("/usr/share/color-schemes");
  QDir colorsNixDir("/var/run/current-system/sw/share/color-schemes");
  if (colorsLocalDir.exists()) {
    colorsLocalDir.setNameFilters(QStringList() << "*.colors");
    colorsLocalDir.setFilter(QDir::Files);
    colorsLocalDir.setSorting(QDir::Name);
    QList<QFileInfo> colorSchemesLocal = colorsLocalDir.entryInfoList();
    QStringList colorSchemesLocalNames;
    for (int i = 0; i < colorSchemesLocal.size(); i++) {
      colorSchemesLocalNames.append(colorSchemesLocal.at(i).baseName());
    }
    colorSchemesNames = colorSchemesNames + colorSchemesLocalNames;
  }
  if (colorsSystemDir.exists()) {
    colorsSystemDir.setNameFilters(QStringList() << "*.colors");
    colorsSystemDir.setFilter(QDir::Files);
    colorsSystemDir.setSorting(QDir::Name);
    QList<QFileInfo> colorSchemesSystem = colorsSystemDir.entryInfoList();
    QStringList colorSchemesSystemNames;
    for (int i = 0; i < colorSchemesSystem.size(); i++) {
      colorSchemesSystemNames.append(colorSchemesSystem.at(i).baseName());
    }
    colorSchemesNames = colorSchemesNames + colorSchemesSystemNames;
  }
  if (colorsNixDir.exists()) {
    colorsNixDir.setNameFilters(QStringList() << "*.colors");
    colorsNixDir.setFilter(QDir::Files);
    colorsNixDir.setSorting(QDir::Name);
    QList<QFileInfo> colorSchemesNix = colorsNixDir.entryInfoList();
    QStringList colorSchemesNixNames;
    for (int i = 0; i < colorSchemesNix.size(); i++) {
      colorSchemesNixNames.append(colorSchemesNix.at(i).baseName());
    }
    colorSchemesNames = colorSchemesNames + colorSchemesNixNames;
  }
  colorSchemesNames.removeDuplicates();
  return colorSchemesNames;
}