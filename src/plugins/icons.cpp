#include "icons.h"

void Icons::setTheme(QString iconTheme) {
  process = new QProcess;

  // locate plasma-changeicons program
  QString locateProgram = "whereis";
  QStringList programToLocate = {"plasma-changeicons"};

  process->start(locateProgram, programToLocate);
  process->waitForFinished();

  QString program(process->readAllStandardOutput());
  program.replace("plasma-changeicons: ", "");
  program.replace("\n", "");

  // apply the icon theme
  QStringList arguments{iconTheme};
  process->start(program, arguments);
}
