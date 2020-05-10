#include "headers/wallpaper.h"

Wallpaper::Wallpaper()
{

}

void Wallpaper::setWallpaper(QString wallFile)
{
    bus = new QDBusConnection(QDBusConnection::sessionBus());
    plasma = new QDBusInterface("org.kde.plasmashell", "/PlasmaShell", "org.kde.PlasmaShell", *bus);
    QString eval = "evaluateScript";
    QString script = "var allDesktops = desktops();"\
                      "print (allDesktops);"\
                      "for (i=0;i<allDesktops.length;i++) {"\
                      "d = allDesktops[i];"\
                      "d.wallpaperPlugin = \"org.kde.image\";"\
                      "d.currentConfigGroup = Array(\"Wallpaper\", \"org.kde.image\", \"General\");"\
                      "d.writeConfig(\"Image\", \"file://" + wallFile + "\")}";
    QList<QVariant> args;
    args.insert(0, script);
    plasma->call(eval).setArguments(args);
}
