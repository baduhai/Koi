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

    QSettings *settings;

    QStringList getPlasmaStyles(void);

    QStringList getColorSchemes(void);

    QStringList getIconThemes(void);

    QStringList getCursorThemes(void);

    QStringList getGtkThemes(void);

    void startupCreate();

    void startupDelete();

    void goLight();

    void goDark();

    void goLightStyle();

    void goDarkStyle();

    void goLightColors();

    void goDarkColors();

    void goLightIcons();

    void goDarkIcons();

    void goLightGtk();

    void goDarkGtk();

    void goLightWall();

    void goDarkWall();

private:

};

#endif // UTILS_H
