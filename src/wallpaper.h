#ifndef WALLPAPER_H
#define WALLPAPER_H

#include <QString>
#include <QtDBus>

class Wallpaper
{
public:
    Wallpaper();
    ~Wallpaper();

    void setWallpaper(const QVariant& wallFile);

private:
    QDBusConnection *bus{};
};

#endif // WALLPAPER_H
