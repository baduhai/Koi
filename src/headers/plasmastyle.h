#ifndef PLASMASTYLE_H
#define PLASMASTYLE_H
// Qt libraries
#include <QString>
#include <QProcess>
#include <QDir>
// KF libraries
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
