#include "headers/icons.h"

Icons::Icons()
{

}

void Icons::setIcons(QString iconTheme)
{
    iconProcess = new QProcess;

    // locate plasma-changeicons program
    QString locateProgram = "whereis";
    QStringList programToLocate = {"plasma-changeicons"};

    iconProcess->start(locateProgram, programToLocate);
    iconProcess->waitForFinished();

    QString program(iconProcess->readAllStandardOutput());
    program.replace("plasma-changeicons: ", "");
    program.replace("\n", "");

    // apply the icon theme
    QStringList arguments{iconTheme};
    iconProcess->start(program, arguments);
}
