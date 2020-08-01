#ifndef KVANTUMSTYLE_H
#define KVANTUMSTYLE_H

#include <QString>
#include <QProcess>

class KvantumStyle
{
public:
    KvantumStyle();

    void setKvantumStyle(QString KvantumStyle);

private:
    QProcess *kvantumStyleProcess;
};
#endif //KVANTUMSTYLE_H