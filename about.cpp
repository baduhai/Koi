#include "about.h"
#include "ui_about.h"

#include <QUrl>
#include <QDesktopServices>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    this->setFixedSize(505, 330);
    QApplication::setApplicationName("Koi");
    ui->titleLabel->setText(QApplication::applicationName());
    QString pgrmVersion = "Version ";
    QApplication::setApplicationVersion("0.1.1");
    ui->verLabel->setText(pgrmVersion + QApplication::applicationVersion());
    QString qtVersion = "<ul><li>Qt ";
            qtVersion += qVersion();
            qtVersion += "</li></ul>";
    ui->qtVerLabel->setText(qtVersion);
    ui->qtVerLabel->setTextFormat(Qt::RichText);
    QString kfVersion = "<ul><li>KDE Frameworks ";
            kfVersion += KCoreAddons::versionString();
            kfVersion += "</li></ul>";
    ui->kf5Version->setText(kfVersion);
    ui->kf5Version->setTextFormat(Qt::RichText);
    QString bosmaLabel = "<ul><li><a href=\"https://github.com/Bosma/Scheduler\">Bosma Scheduler</a></li></ul>";
    ui->bosmaLib->setText(bosmaLabel);
    ui->bosmaLib->setTextFormat(Qt::RichText);
    ui->bosmaLib->setOpenExternalLinks(1);
    QString ctplLabel = "<ul><li><a href=\"https://github.com/vit-vit/CTPL\">CTPL</a></li></ul>";
    ui->ctpLib->setText(ctplLabel);
    ui->ctpLib->setTextFormat(Qt::RichText);
    ui->ctpLib->setOpenExternalLinks(1);
    ui->tabWidget->setCurrentIndex(0);
}

About::~About()
{
    delete ui;
}

void About::on_closeBtn_clicked()
{
    this->setVisible(0);
}

void About::on_licenseBtn_clicked()
{
    auto* licence = new License(this);
    licence->open();
}

void About::on_wWebsiteBtn_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/baduhai", QUrl::TolerantMode));
}
