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
    //Constructeur
    explicit MainWindow(QWidget *parent = 0);
    //Destructeur
    ~MainWindow();
    //Emet le signal de mise à jour des layout de dataViewer pour mettre a jour les layout des optionsViewer
    void updateLayoutsOptions();
    void updateLayoutsViewers();
    void removeColumn(QString codeObject, QString currentConfigName, int index);
    void setColumnChangeIndic(QString choice, int index);
    //Permet a d'autres classes (descriptiveCard) d'emettre le signal de suppression de la colonne
    void triggerSignalChangeColumn();
    //Reset kesToTreat après la mise à jour du modele
    void resetKeysToTreat();

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
    //On définit les attributs pour les objets dont on a besoin
    DataViewer *dataViewer;
    DataManager *dataManager;
    FileReader *fileReader;
    FileWriter *fileWriter;
    PasswordForm *passwordForm;
    QStringList keysToTreat;
    //Choix de l'objet à ajouter, renseigné lors du clic sur le menu
    QString choiceAddObject;
    //Code object de la colonne a traiter dans les modeles de données
    QString columnToTreatCodeObject;
    //Nom de la configuration de la colonne a traiter dans les modeles de données
    QString columnToTreatConfigName;
    //Numéro de colonne de l'index a supprimer des modeles de données
    int columnToRemoveIndex;





};

#endif // MAINWINDOW_H
