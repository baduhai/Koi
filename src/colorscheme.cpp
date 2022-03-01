#include "headers/colorscheme.h"

ColorScheme::ColorScheme()
{

}

void ColorScheme::setColorScheme(QString colorScheme)
{
    colorSchemeProcess = new QProcess;
    QString program = "/usr/bin/plasma-apply-colorscheme";
    QStringList arguments{colorScheme};
    colorSchemeProcess->start(program, arguments);
}
