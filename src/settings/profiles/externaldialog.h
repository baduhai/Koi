#ifndef EXTERNALDIALOG_H
#define EXTERNALDIALOG_H

#include <QWidget>

namespace Ui
{
class ExternalDialog;
}

class ExternalDialog: public QWidget
{
Q_OBJECT

public:
    explicit ExternalDialog(QWidget *parent = nullptr);
    ~ExternalDialog();

private:
    Ui::ExternalDialog *ui;
};

#endif // EXTERNALDIALOG_H
