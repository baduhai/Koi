#ifndef USEGLOBAL_H
#define USEGLOBAL_H

#include <QString>
#include <QProcess>

class UseGlobal
{
private:
    QProcess *useGlobalProcess;

public:
    UseGlobal();

    void setGlobalTheme(QString themename);
};
#endif // !USEGLOBAL_H