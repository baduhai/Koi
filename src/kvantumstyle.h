#ifndef KVANTUMSTYLE_H
#define KVANTUMSTYLE_H

#include <QString>
#include <QProcess>

class KvantumStyle
{
public:
    KvantumStyle();
    ~KvantumStyle();
    void setKvantumStyle(QString KvantumStyle);
    static QStringList getKvantumStyles();
private:
    QProcess *kvantumStyleProcess;
};
#endif //KVANTUMSTYLE_H