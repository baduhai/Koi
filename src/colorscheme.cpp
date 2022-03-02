#include "headers/colorscheme.h"

ColorScheme::ColorScheme()
{

}

void ColorScheme::setColorScheme(QString colorScheme)
{

    QString locateProgram = "whereis";
    QStringList programToLocate = {"plasma-apply-colorscheme"};

    colorSchemeProcess->start(locateProgram, programToLocate);
    colorSchemeProcess->waitForFinished();

    QString program(colorSchemeProcess->readAllStandardOutput());
    program.replace("plasma-apply-colorscheme: ", "");
    program.replace("\n", "");

    QStringList arguments{colorScheme};
    colorSchemeProcess->start(program, arguments);
}
