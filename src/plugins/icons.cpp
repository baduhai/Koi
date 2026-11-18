#include "icons.h"
#include "../headers/utils.h"

#include <QProcess>

void Icons::setTheme(QString iconTheme) {

  // locate plasma-changeicons program
  QProcess process;
  QString locateProgram = "whereis";
  QStringList programToLocate = {"plasma-changeicons"};

  process.start(locateProgram, programToLocate);
  process.waitForFinished();

  QString program(process.readAllStandardOutput());
  program.replace("plasma-changeicons: ", "");
  program.replace("\n", "");

  // apply the icon theme
  QStringList arguments{iconTheme};
  QProcess::startDetached(program, arguments);
}

QStringList Icons::getThemes() {
  const QStringList searchPaths = Utils::systemDataSearchPaths(
    QStringLiteral("icons"),
    {QDir::homePath() + QStringLiteral("/.icons")});

  QStringList iconThemes;
  for (const QString &dirPath : searchPaths) {
    const QDir dir(dirPath);
    iconThemes.append(dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
  }

  iconThemes.removeDuplicates();
  iconThemes.sort();
  return iconThemes;
}
