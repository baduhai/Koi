#include "colorscheme.h"
#include "../headers/utils.h"

#include <QProcess>

void ColorScheme::setTheme(QString themeName) {
  QProcess process;
  QString locateProgram = "whereis";
  QStringList programToLocate = {"plasma-apply-colorscheme"};

  process.start(locateProgram, programToLocate);
  process.waitForFinished();

  QString program(process.readAllStandardOutput());
  program.replace("plasma-apply-colorscheme: ", "");
  program.replace("\n", "");

  QStringList arguments{themeName};
  QProcess::startDetached(program, arguments);
}

QStringList ColorScheme::getThemes() {
  const QStringList searchPaths =
  Utils::systemDataSearchPaths(QStringLiteral("color-schemes"));

  QStringList colorSchemesNames;
  for (const QString &dirPath : searchPaths) {
    QDir dir(dirPath);
    dir.setNameFilters({QStringLiteral("*.colors")});
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name);

    const QList<QFileInfo> entries = dir.entryInfoList();
    for (const QFileInfo &entry : entries) {
      colorSchemesNames.append(entry.baseName());
    }
  }

  colorSchemesNames.removeDuplicates();
  colorSchemesNames.sort();
  return colorSchemesNames;
}
