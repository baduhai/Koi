#include "headers/kvantumstyle.h"

KvantumStyle::KvantumStyle()
{
}
/*  enables the kvantum theme by calling the command 
    kvantummanger with the argument "--set"
    */
void KvantumStyle::setKvantumStyle(QString kvantumStyle)
{
    kvantumStyleProcess = new QProcess;
    QString program = "/usr/bin/kvantummanager";
    QStringList arguments{"--set", kvantumStyle};
    kvantumStyleProcess->start(program, arguments);
}