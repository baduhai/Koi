#include "headers/script.h"

Script::Script()
{

}

void Script::runScript(QString scriptFile)
{
    scriptProcess = new QProcess;
    QString bash = "/bin/bash";
    QStringList arguments{"-c", scriptFile};
    scriptProcess->start(bash, arguments);
    scriptProcess->waitForFinished();
    scriptProcess->close();
}
