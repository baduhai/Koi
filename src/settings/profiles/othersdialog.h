#ifndef OTHERSDIALOG_H
#define OTHERSDIALOG_H

#include <QWidget>

namespace Ui {
class OthersDialog;
}

class OthersDialog : public QWidget
{
    Q_OBJECT

public:
    explicit OthersDialog(QWidget *parent = nullptr);
    ~OthersDialog();

private:
    Ui::OthersDialog *ui;
};

#endif // OTHERSDIALOG_H
