#include "stylesdialog.h"
#include "ui_stylesdialog.h"

StylesDialog::StylesDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StylesDialog)
{
    ui->setupUi(this);
}

StylesDialog::~StylesDialog()
{
    delete ui;
}
