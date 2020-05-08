#include "headers/wallpaper.h"

Wallpaper::Wallpaper()
{

}

void Wallpaper::setWallpaper(QString)
{
    bus = new QDBusConnection(QDBusConnection::sessionBus());
    plasma = new QDBusInterface
}
