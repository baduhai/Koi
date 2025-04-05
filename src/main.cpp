#include "headers/mainwindow.h"
#include "headers/utils.h"
#include "headers/dbusinterface.h"

#include <iostream>
#include <memory>
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

std::unique_ptr<QLocalServer> createDummyNetwork(QString netName)
{
    auto server = std::make_unique<QLocalServer>();
    server->setSocketOptions(QLocalServer::WorldAccessOption);
    server->listen(netName);
    return server;
}

int main(int argc, char *argv[])
{
    if (isAlreadyRunning("koiDummyNetwork"))
    {
        std::cout << "Another instance of Koi is already running" << std::endl;
    }
    else
    {
        const auto dummyServer = createDummyNetwork("koiDummyNetwork");
        Utils utils;
        utils.initialiseSettings();
        QApplication a(argc, argv);
        MainWindow w;
        if (utils.settings->value("start-hidden").toBool() == 0)
        {
            w.show();
        }
        KoiDbusInterface dbusIf(&a);
        return a.exec();
    }
}
