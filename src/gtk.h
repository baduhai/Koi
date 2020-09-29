#ifndef GTK_H
#define GTK_H

#include <QString>
#include <QtDBus>
#include <QFileInfo>

class Gtk
{
public:
    Gtk();

    void setGtk(const QString& gtkTheme);
    static QStringList getGtkThemes();
private:
};

#endif // GTK_H
