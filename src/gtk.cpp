#include "headers/gtk.h"

Gtk::Gtk()
{

}

void Gtk::setGtk(QString gtkTheme)
{
    bus = new QDBusConnection(QDBusConnection::sessionBus());
    interface = new QDBusInterface("org.kde.GtkConfig", "/GtkConfig", "org.kde.GtkConfig", *bus);
    interface->call("setGtk2Theme", gtkTheme);
    interface->call("setGtk3Theme", gtkTheme);
}
