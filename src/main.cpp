#include "headers/mainwindow.h"
#include "headers/utils.h"
#include "headers/dbusinterface.h"
#include "headers/runguard.h"

#include <iostream>
#include <memory>
#include <QApplication>

int main(int argc, char *argv[])
{
    RunGuard rg("koiDummyNetwork");
    if (!rg.tryToRun())
    {
        std::cout << "Another instance of Koi is already running" << std::endl;
        return 1;
    }

    Utils utils;
    utils.initialiseSettings();
    QApplication a(argc, argv);
    MainWindow w(nullptr,&utils);
    if (utils.settings->value("start-hidden").toBool() == 0)
    {
        w.initSettingsInterface();
        w.show();
    }
    utils.startupCheck();
    KoiDbusInterface dbusIf(&a,&utils);
    return a.exec();
}
