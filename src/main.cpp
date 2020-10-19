#include "mainwindow.h"
#include "utils.h"

#include <iostream>
#include <QApplication>
#include <QLocalSocket>
#include <QLocalServer>

bool isAlreadyRunning(const QString& netName) {
    QLocalSocket socket;
    socket.connectToServer(netName);
    bool isOpen = socket.isOpen();
    socket.close();
    return isOpen;
}

void createDummyNetwork(const QString& netName) {
    QLocalServer *server = new QLocalServer;
    server->setSocketOptions(QLocalServer::WorldAccessOption);
    server->listen(netName);
}

int main(int argc, char *argv[]) {
    QCoreApplication::setOrganizationName("koi");
    QCoreApplication::setApplicationName("koi");

    if (isAlreadyRunning("koiDummyNetwork")) {
        std::cout << "Another instance of Koi is already running" << std::endl;
    } else {
        createDummyNetwork("koiDummyNetwork");

        QApplication a(argc, argv);
        MainWindow w;
        w.showWindow();

        return a.exec();
    }
}
