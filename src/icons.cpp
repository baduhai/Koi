#include "headers/icons.h"

Icons::Icons()
{

}

void Icons::setIcons(QString iconTheme)
{
    iconProcess = new QProcess;
    QString program = "/usr/lib/plasma-changeicons";
    QStringList arguments{iconTheme};
    iconProcess->start(program, arguments);
}
