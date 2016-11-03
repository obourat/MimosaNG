#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

//On inclut les classes qui intéragissent avec MainWindow
class DataViewer;
class DataManager;
class FileReader;
class FileWriter;
class PasswordForm;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //Constructeur
    explicit MainWindow(QWidget *parent = 0);
    //Destructeur
    ~MainWindow();


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




};

#endif // MAINWINDOW_H
