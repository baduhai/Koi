#include "headers/mainwindow.h"
#include "headers/utils.h"

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
    QLocalServer* server = new QLocalServer;
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

        LnfLogic logic;

        QApplication a(argc, argv);
        MainWindow w;
        if (utils.settings->value("start-hidden").toBool() == 0)
        {
            w.show();
        }
        return a.exec();
    }
}

