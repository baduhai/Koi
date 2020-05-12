#ifndef ICONS_H
#define ICONS_H

#include <QString>
#include <QProcess>

class Icons
{
public:
    Icons();

    void setIcons(QString iconTheme);

private:
    QProcess *iconProcess;
};

#endif // ICONS_H
