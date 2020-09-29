#ifndef WALLPAPER_H
#define WALLPAPER_H

#include <QString>
#include <QtDBus>

class Wallpaper
{
public:
    Wallpaper();

    void setWallpaper(QString wallFile);

private:
    QDBusConnection *bus;
};

#endif // WALLPAPER_H
