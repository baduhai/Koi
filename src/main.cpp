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
	if (isAlreadyRunning("koiDummyNetwork")) {
		std::cout << "Another instance of Koi is already running" << std::endl;
	}
	else {
		createDummyNetwork("koiDummyNetwork");

		QApplication a(argc, argv);
		MainWindow w;

		QSettings s(QDir::homePath() + "/.config/koirc", QSettings::IniFormat);
		if (!s.value("start-hidden").toBool()) {
			w.show();
		}
		return a.exec();
	}
}
