#include "headers/license.h"
#include "ui/ui_license.h"

License::License(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::License)
{
    QFile license(":/resources/license.txt");
    license.open(QFile::ReadOnly);
    QString licenseContent = QString::fromUtf8(license.readAll());
    ui->setupUi(this);
    ui->licenseField->setPlainText(licenseContent);
}

License::~License()
{
    delete ui;
}

void License::on_closeBtn_clicked()
{
    this->setVisible(0);
}
