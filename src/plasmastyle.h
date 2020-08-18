#ifndef PLASMASTYLE_H
#define PLASMASTYLE_H
// Qt libs
#include <QString>
#include <QProcess>
#include <QDir>
// KF5 libs
#include <KSharedConfig>
#include <KConfigGroup>

class PlasmaStyle
{
public:
    PlasmaStyle();

    static QStringList getPlasmaStyles();
};

#endif // PLASMASTYLE_H
