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
