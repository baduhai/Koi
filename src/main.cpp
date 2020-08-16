#include "mainwindow.h"
#include "utils.h"

#include <iostream>
#include <QApplication>
#include <QLocalSocket>
#include <QLocalServer>

bool isAlreadyRunning(QString netName)
{
    QLocalSocket socket;
    socket.connectToServer(netName);
    bool isOpen = socket.isOpen();
    socket.close();
    return isOpen;
}

void createDummyNetwork(QString netName)
{
    QLocalServer *server = new QLocalServer;
    server->setSocketOptions(QLocalServer::WorldAccessOption);
    server->listen(netName);
}

int main(int argc, char *argv[])
{
    if (isAlreadyRunning("koiDummyNetwork"))
    {
        std::cout << "Another instance of Koi is already running" << std::endl;
    }
    else
    {
        createDummyNetwork("koiDummyNetwork");
        Utils utils;
        utils.initialiseSettings();

//        LnfLogic logic{};
//        LnfListModel *themeList = logic.lnfList();
//        if (!(themeList->themeExists("Koi-Light")))
//        {
//            logic.createNewTheme("Koi-Light", "Koi-Light", "Light theme for koi", "Bahduai & Da-viper", "none", "gpl", "www.github.com");
//        }
//
//        if (!(themeList->themeExists("Koi-Dark")))
//        {
//            logic.createNewTheme("Koi-Dark","Koi-Dark", "Dark theme for koi", "Bahduai & Da-viper", "none", "gpl", "www.github.com");
//
//        }

        if(!utils.themeExists(QStringLiteral("Koi-Dark"))){
           utils.createNewTheme("Koi-Dark", "Koi-Dark", "Dark theme for koi", "Bahduai & Da-viper", "none", "gpl", "www.github.com");
        }
        if(!utils.themeExists(QStringLiteral("Koi-Light"))){
           utils.createNewTheme("Koi-Light", "Koi-Light", "Light theme for koi", "Bahduai & Da-viper", "none", "gpl", "www.github.com");
        }
            /* QList<ThemeInfo> logicThemes = logic.lnfList()->getThemes();

        for ( ThemeInfo temp  : logicThemes)
        {
            if(temp.name == "Koi-Light"){
                 
            }
        }  */

            QApplication a(argc, argv);
            MainWindow w;
            if (utils.settings->value("start-hidden").toBool() == 0)
            {
                w.show();
            }
            return a.exec();
        }
    }
