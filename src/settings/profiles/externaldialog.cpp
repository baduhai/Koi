#include "externaldialog.h"
#include "ui_externaldialog.h"

ExternalDialog::ExternalDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExternalDialog)
{
    ui->setupUi(this);
}

ExternalDialog::~ExternalDialog()
{
    delete ui;
}
