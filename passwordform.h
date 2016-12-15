#ifndef PASSWORDFORM_H
#define PASSWORDFORM_H

#include <QDialog>
#include <QLineEdit>

class DataManager;

namespace Ui {
class PasswordForm;
}

class PasswordForm : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordForm(DataManager *dataManager, int levelRequested, QWidget *parent = 0);
    ~PasswordForm();

private slots:
    //Slot de validation
    void on_buttonBox_accepted();

private:
    //User interface
    Ui::PasswordForm *ui;

    //Definition des objets pour le traitement des données
    DataManager *dataManager;

    //Indicateur du niveau d'autorisation demandé
    int levelRequested;
    //Définition de l'input de mot de passe
    QLineEdit *password;
};

#endif // PASSWORDFORM_H
