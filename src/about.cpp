#include "headers/about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    QApplication::setApplicationName("Koi");
    ui->titleLabel->setText(QApplication::applicationName());
    QString pgrmVersion = "Version ";
    QApplication::setApplicationVersion("0.1");
    ui->verLabel->setText(pgrmVersion + QApplication::applicationVersion());
    QString qtVersion = "<ul><li>Qt ";
            qtVersion += qVersion();
            qtVersion += "</li></ul>";
    ui->qtVerLabel->setText(qtVersion);
    ui->qtVerLabel->setTextFormat(Qt::RichText);
    QString kfVersion = "<ul><li>KDE Framworks ";
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
