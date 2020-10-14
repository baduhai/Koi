#include <QtCore/QDir>
#include "kvantumstyle.h"

KvantumStyle::KvantumStyle()
{
}
KvantumStyle::~KvantumStyle()
= default;
/*  enables the kvantum theme by calling the command 
    kvantummanger with the argument "--set"
    */
void KvantumStyle::setKvantumStyle(QString kvantumStyle)
{
    kvProcess = new QProcess;
    QString program = "/usr/bin/kvantummanager";
    QStringList arguments{"--set", std::move(kvantumStyle)};
    QObject::connect(kvProcess, qOverload<int,QProcess::ExitStatus>(&QProcess::finished),kvProcess, &QProcess::deleteLater);
    kvProcess->start(program, arguments);
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
