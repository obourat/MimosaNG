#ifndef EXPORTFORM_H
#define EXPORTFORM_H

#include <QDialog>

class DataViewer;
class DataManager;

namespace Ui {
class ExportForm;
}

class ExportForm : public QDialog
{
    Q_OBJECT

public:
    explicit ExportForm(DataManager *dataManager, QStringList keysList, QString codeObjet, QWidget *parent = 0);
    ~ExportForm();

private slots:
    //Slot de validation de l'exportation
    void on_confirmButtonBox_accepted();
    //Slot de validation du bouton explorateur de fichier
    void on_browseButton_released();

private:
    //User interface
    Ui::ExportForm *ui;

    //Définition des objets nécessaires à la gestion des données
    DataManager *dataManager;

    //Liste des clés à exporter
    QStringList keysList;
    //Code objet des éléments à exporter
    QString codeObjet;
    //Liste des configurations disponibles du type d'objet
    QList<QMap<QString, QString> > configurationsKeys;
};

#endif // EXPORTFORM_H
