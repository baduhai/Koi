#ifndef PLASMASTYLE_H
#define PLASMASTYLE_H
// Qt libs
#include <QString>
#include <QProcess>
#include <QDir>
// KF libs
#include <KSharedConfig>
#include <KConfigGroup>

class PlasmaStyle
{
public:
    PlasmaStyle();

    void setPlasmaStyle(QString plasmaStyle);

private:
    QProcess *styleProcess;
};

#endif // PLASMASTYLE_H
