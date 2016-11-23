#ifndef PRINTFORM_H
#define PRINTFORM_H

#include <QDialog>

namespace Ui {
class PrintForm;
}

class PrintForm : public QDialog
{
    Q_OBJECT

public:
    explicit PrintForm(QWidget *parent = 0);
    ~PrintForm();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PrintForm *ui;
};

#endif // PRINTFORM_H
