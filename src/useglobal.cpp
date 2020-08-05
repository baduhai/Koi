#include "useglobal.h"

UseGlobal::UseGlobal()
{
}

void UseGlobal::setGlobalTheme(QString themename)
{
    useGlobalProcess = new QProcess;
    QString command = "lookandfeeltool";
    QStringList arguments = {"-a ", themename};
    useGlobalProcess->start(command, arguments);
}