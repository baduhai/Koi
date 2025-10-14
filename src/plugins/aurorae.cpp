#include "aurorae.h"

#include <QProcess>

/*
If ever needed, the actual source of plasma-apply-aurorae is:
https://invent.kde.org/plasma/kwin/-/blob/master/src/kcms/decoration/kwin-applywindowdecoration.cpp
The program actually wraps this 'kwin-applywindowdecoration' tool.
*/

/*
The following 2 methods are quite generic and could be used for other plugins
too.
*/
QString get_program_output(QString program, QStringList &params) {
  QProcess process;

  process.start(program, params);
  process.waitForFinished();

  QString program_output(process.readAllStandardOutput());

  return program_output;
}

QString get_program_path(QString programToLocate) {
  QString locateProgram = "whereis";
  QStringList locateProgramArgs = {programToLocate};

  QString program_path = get_program_output(locateProgram, locateProgramArgs);

  program_path.replace(programToLocate + ": ", "");
  program_path.replace("\n", "");

  return program_path;
}

QString get_plasma_apply_aurorae_path() {
  return get_program_path("plasma-apply-aurorae");
}

void Aurorae::setTheme(QString themeName) {
  QString program = get_plasma_apply_aurorae_path();
  QStringList arguments{themeName};
  QProcess::startDetached(program, arguments);
}

QStringList Aurorae::getThemes() {
  QStringList auroraeThemes;

  // Get the output of: plasma-apply-aurorae --list-themes
  QString program = get_plasma_apply_aurorae_path();
  QStringList programArgs{"--list-themes"};
  QString programOutput = get_program_output(program, programArgs);

  /*
  Parse the output to extract the theme names.
  The standard output of the program is:

    You have the following KWin window decoration themes on your system:
    * Plastik (theme name: kwin4_decoration_qml_plastik)
    * Layan (theme name: __aurorae__svg__Layan)
    * Layan-light (theme name: __aurorae__svg__Layan-light)
    * Layan-solid (theme name: __aurorae__svg__Layan-solid)
    * Breeze (theme name: Breeze - current theme for this Plasma session)
    * Oxygen (theme name: Oxygen)

  So we want to match the themes via the regex:

    .*theme name: ([\w-]+).*

  This regex will detect the group and identify the theme.
  \w is equivalent to '[a-zA-Z0-9_]'; we add '-' since some themes of mine have
  dashes in the name.
  */

  QRegularExpression themeRegex(".*theme name: ([\\w-]+).*");

  QStringList outputLines = programOutput.split("\n");
  for (QString outputLine : outputLines) {
    QRegularExpressionMatch regexMatch = themeRegex.match(outputLine);
    if (regexMatch.isValid()) {
      QString themeName = regexMatch.captured(1);
      auroraeThemes << themeName;
    }
  }

  auroraeThemes.removeDuplicates();
  auroraeThemes.sort();
  // Somehow we get an empty line. Disable it.
  auroraeThemes.removeAll("");
  return auroraeThemes;
}
