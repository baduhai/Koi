#ifndef COLORSCHEME_H
#define COLORSCHEME_H
// Qt libraries
#include <QString>
#include <QProcess>

class ColorScheme
{
public:
    ColorScheme();

    void setColorScheme(QString colorScheme);

private:
    QProcess *colorSchemeProcess;
};

#endif // COLORSCHEME_H
