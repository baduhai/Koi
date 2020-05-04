#ifndef UTILS_H
#define UTILS_H

#include <QStringList>
#include <QDir>
#include <QProcess>
#include <QFileDialog>
#include <QSettings>

class Utils
{
public:
    Utils();

    QStringList getPlasmaStyles(void);

    QStringList getColorSchemes(void);

    QStringList getIconThemes(void);

    QStringList getCursorThemes(void);

    QStringList getGtkThemes(void);

private:


};

#endif // UTILS_H
