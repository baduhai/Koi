#include "headers/script.h"

Script::Script()
{

}

void Script::runScript(QString scriptFile)
{
    scriptProcess = new QProcess;

    QString locateProgram = "which";
    QStringList programToLocate = {"bash"};

    scriptProcess->start(locateProgram, programToLocate);
    scriptProcess->waitForFinished();

    QString program(scriptProcess->readAllStandardOutput());
    program.replace("\n", "");
    
    QStringList arguments{"-c", scriptFile};
    scriptProcess->start(program, arguments);
    scriptProcess->waitForFinished();
    scriptProcess->close();
}
