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
    //Crée le fichier de configurations
    void makePrintConfigFile();
    //Crée le fichier de données
    void makePrintDataFile();

private slots:
    //Slot de validation
    void on_buttonBox_accepted();

private:
    //User interface
    Ui::PrintForm *ui;

    //Vue pour les données
    QTableView *view;
};

#endif // PRINTFORM_H
