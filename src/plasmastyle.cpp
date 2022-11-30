#include "headers/plasmastyle.h"

PlasmaStyle::PlasmaStyle()
{

}

void PlasmaStyle::setPlasmaStyle(QString plasmaStyle)
{
    styleProcess = new QProcess;
    QString style = "/usr/bin/plasma-apply-desktoptheme";
    QStringList styleArgs = {plasmaStyle};
    styleProcess->start(style, styleArgs);
    styleProcess->waitForFinished();
    styleProcess->close();
}
