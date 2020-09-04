#ifndef STYLESDIALOG_H
#define STYLESDIALOG_H

#include <QWidget>

namespace Ui {
class StylesDialog;
}

class StylesDialog : public QWidget
{
    Q_OBJECT

public:
    explicit StylesDialog(QWidget *parent = nullptr);
    ~StylesDialog();

private:
    Ui::StylesDialog *ui;
};

#endif // STYLESDIALOG_H
