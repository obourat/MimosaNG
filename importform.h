#ifndef IMPORTFORM_H
#define IMPORTFORM_H

#include <QDialog>
#include <QButtonGroup>

class DataManager;

namespace Ui {
class ImportForm;
}

class ImportForm : public QDialog
{
    Q_OBJECT

public:
    explicit ImportForm(DataManager *dataManager, QString codeObjet, QWidget *parent = 0);
    ~ImportForm();

private slots:
    //Slot de validation de l'importation
    void on_buttonBox_accepted();
    //Slot de validation du bouton explorateur de fichier
    void on_browsePushButton_released();

private:
    //User interface
    Ui::ImportForm *ui;

    //Définition des objets nécessaire pour la gestion des données
    DataManager *dataManager;

    //Code objet des éléments à importer
    QString codeObjet;
    //Liste des configurations disponibles du type d'objet
    QList<QMap<QString, QString> > configurationsKeys;
    //Définition du groupe de boutons pour les otpions de la fiche
    QButtonGroup *buttonGroup;
};

#endif // IMPORTFORM_H
