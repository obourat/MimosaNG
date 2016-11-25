#ifndef PRINTFORM_H
#define PRINTFORM_H

#include <QDialog>
#include <QTableView>

class Model;

namespace Ui {
class PrintForm;
}

class PrintForm : public QDialog
{
    Q_OBJECT

public:
    explicit PrintForm(QTableView *view, QWidget *parent = 0);
    ~PrintForm();

private:
    void makePrintConfigFile();
    void makePrintDataFile();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PrintForm *ui;
    //Model *model;
    QTableView *view;
};

#endif // PRINTFORM_H
