#ifndef COLORSCHEME_H
#define COLORSCHEME_H
// Qt libs
#include <QString>
#include <QtDBus>
#include <QFileInfo>
#include <QProcess>
// KF5 libs
#include <KSharedConfig>
#include <KConfigGroup>

class ColorScheme
{
public:
    ColorScheme();

    void setColorScheme(const QString &colorScheme);
    static QStringList getColorSchemes();

private:
    QProcess *breezeGtkProcess;
};

#endif // COLORSCHEME_H
