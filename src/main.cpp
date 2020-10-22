#include "mainwindow.h"

#include <QApplication>
#include <QLocalSocket>
#include <QLocalServer>

bool isAlreadyRunning(const QString &netName)
{
    QLocalSocket socket;
    socket.connectToServer(netName);
    bool isOpen = socket.isOpen();
    socket.close();
    return isOpen;
}

void createDummyNetwork(const QString &netName)
{
    QLocalServer *server = new QLocalServer;
    server->setSocketOptions(QLocalServer::WorldAccessOption);
    server->listen(netName);
}

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("koi");
    QCoreApplication::setApplicationName("koi");
    QApplication a(argc, argv);

    if (isAlreadyRunning("koiDummyNetwork")) {
        qDebug() << "Another instance of Koi is already running";
        QMessageBox msgBox;
        msgBox.setIconPixmap(QPixmap(":/resources/icons/koi.svg"));
        msgBox.setText("Another instance of Koi is already running \n Check your system tray.");
        return msgBox.exec();
    }
    else {
        createDummyNetwork("koiDummyNetwork");

        MainWindow w;
        w.showWindow();

        return QApplication::exec();
    }
}
