#include <QtCore/QDir>
#include <utility>
#include "kvantumstyle.h"

KvantumStyle::KvantumStyle()
{
}
/*  enables the kvantum theme by calling the command 
    kvantummanger with the argument "--set"
    */
void KvantumStyle::setKvantumStyle(QString kvantumStyle)
{
    kvantumStyleProcess = new QProcess;
    QString program = "/usr/bin/kvantummanager";
    QStringList arguments{"--set", std::move(kvantumStyle)};
    kvantumStyleProcess->start(program, arguments);
}

QStringList KvantumStyle::getKvantumStyles() // Get all available kvantum styles
{
    QDir kvantumStyleLocalDir(QDir::homePath() + "/.config/Kvantum");
    QDir kvantumStyleSystemDir("/usr/share/Kvantum");
    QStringList kvantumStyles;
    if (kvantumStyleLocalDir.exists()){
        kvantumStyles.append(kvantumStyleLocalDir.entryList(QDir::Dirs  | QDir::NoDotAndDotDot  ));
    }
    if(kvantumStyleSystemDir.exists()){
        kvantumStyles.append(kvantumStyleSystemDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot  ));
    }
    kvantumStyles.removeDuplicates();
    return kvantumStyles;
}
