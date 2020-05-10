#include "headers/mainwindow.h"
#include "headers/utils.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    Utils utils;
    utils.initialiseSettings();
    QApplication a(argc, argv);
    MainWindow w;
    if (utils.settings->value("start-hidden").toBool() == 0)
    {
        w.show();
    }
    return a.exec();
}
