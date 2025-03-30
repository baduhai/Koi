#include "script.h"

#include <QProcess>

void Script::setTheme(QString scriptFile) {

  QProcess process;
  QString locateProgram = "which";
  QStringList programToLocate = {"bash"};

  process.start(locateProgram, programToLocate);
  process.waitForFinished();

  QString program(process.readAllStandardOutput());
  program.replace("\n", "");

  QStringList arguments{"-c", scriptFile};
  QProcess::execute(program, arguments);
}