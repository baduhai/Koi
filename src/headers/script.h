#ifndef SCRIPT_H
#define SCRIPT_H

#include <QString>
#include <QProcess>

class Script
{
public:
    Script();

    void runScript(QString scriptFile);

private:
    QProcess *scriptProcess;
};
#endif // SCRIPT_H
