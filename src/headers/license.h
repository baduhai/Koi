#pragma once

// Qt libraries
#include <QDialog>

namespace Ui {
class License;
}

class License : public QDialog {
  Q_OBJECT

public:
  explicit License(QWidget *parent = nullptr);
  ~License();

private slots:
  void on_closeBtn_clicked();

private:
  Ui::License *ui;
};
