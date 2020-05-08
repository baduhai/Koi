#ifndef WALLPAPER_H
#define WALLPAPER_H

#include <QString>
#include <QtDBus>
#include <QDBusInterface>

class Wallpaper
{
public:
    Wallpaper();

    void setWallpaper(QString);

private:
    QDBusConnection *bus;
    QDBusInterface *plasma;
};

#endif // WALLPAPER_H
