#include "script.h"

void Script::setTheme(QString scriptFile) {
  process = new QProcess;

  QString locateProgram = "which";
  QStringList programToLocate = {"bash"};

  process->start(locateProgram, programToLocate);
  process->waitForFinished();

  QString program(process->readAllStandardOutput());
  program.replace("\n", "");

  QStringList arguments{"-c", scriptFile};
  process->start(program, arguments);
  process->waitForFinished();
  process->close();
}