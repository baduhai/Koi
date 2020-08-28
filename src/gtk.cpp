#include "gtk.h"

Gtk::Gtk()
{

}

void Gtk::setGtk(const QString& gtkTheme)
{
    bus = new QDBusConnection(QDBusConnection::sessionBus());
    interface = new QDBusInterface("org.kde.GtkConfig", "/GtkConfig", "org.kde.GtkConfig", *bus);
    interface->call("setGtk2Theme", gtkTheme);
    interface->call("setGtk3Theme", gtkTheme);
}

QStringList Gtk::getGtkThemes() // Get all available gtk themes
{
    QDir gtkLocalDir(QDir::homePath() + "/.themes");
    QDir gtkSystemDir("/usr/share/themes");
    QStringList gtkThemes = gtkLocalDir.entryList(QDir::Dirs) + gtkSystemDir.entryList(QDir::Dirs);
    gtkThemes.removeDuplicates();
    gtkThemes.removeFirst();
    gtkThemes.removeFirst();
    return gtkThemes;
}
