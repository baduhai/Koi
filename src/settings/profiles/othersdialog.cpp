#include "othersdialog.h"
#include "ui_othersdialog.h"

OthersDialog::OthersDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OthersDialog)
{
    ui->setupUi(this);
}

OthersDialog::~OthersDialog()
{
    delete ui;
}
