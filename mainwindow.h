#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class DataViewer;
class DataManager;
class FileReader;
class FileWriter;
class PasswordForm;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //Emet le signal de mise à jour des layout des DataViewers et OptionsViewers
    void updateLayoutsOptions();
    void updateLayoutsViewers();

    //Enleve du modèle la colonne renseignée
    void removeColumn(QString codeObject, QString currentConfigName, int index);
    //Permet a d'autres classes (descriptiveCard) d'emettre le signal de suppression de la colonne
    void triggerSignalChangeColumn();
    //Reset kesToTreat après la mise à jour du modele
    void resetKeysToTreat();
    //Met à jour le nom de l'affaire dans mainWindow
    void updateCaseName();

    //Getters et Setters
    QStringList getKeysToTreat() const;
    void setKeysToTreat(const QString &value);

    QString getChoiceAddObject() const;
    void setChoiceAddObject(const QString &value);

    QString getColumnToTreatCodeObject() const;
    void setColumnToTreatCodeObject(const QString &value);

    QString getColumnToTreatConfigName() const;
    void setColumnToTreatConfigName(const QString &value);

    int getColumnToRemoveIndex() const;
    void setColumnToRemoveIndex(int value);

signals:
    //Signaux de mise à jour des Modèles/Vues
    void signalUpdateLayoutsViewers();
    void signalUpdateLayoutsOptions();
    void signalChangeColumn();
    void signalCaseChanged();

private slots:
    //Slots du clic sur les différents boutons de la MainWindow
    void on_environmentalVariablesButton_released();
    void on_attributesConfigurationsButton_released();
    void on_attributesButton_released();
    void on_caseSelectionButton_released();
    void on_officialsButton_released();
    void on_documentsButton_released();
    void on_consultationButton_released();
    void on_modificationButton_released();
    void on_adminButton_released();

private:
    //User Interface
    Ui::MainWindow *ui;

    //Définition des objets nécessaire pour la gestion des données
    DataViewer *dataViewer;
    DataManager *dataManager;
    FileReader *fileReader;
    FileWriter *fileWriter;
    PasswordForm *passwordForm;

    //Liste des objets à traiter lors d'une modification
    QStringList keysToTreat;
    //Choix de l'objet à ajouter, renseigné lors du clic sur le menu
    QString choiceAddObject;
    //Code object de la colonne a traiter dans les modeles de données
    QString columnToTreatCodeObject;
    //Nom de la configuration de la colonne a traiter dans les modeles de données
    QString columnToTreatConfigName;
    //Numéro de colonne de l'index a supprimer des modeles de données
    int columnToRemoveIndex;
    //Indicateur de mise à jour des fichiers
    int indicUpdateFiles;





};

#endif // MAINWINDOW_H
