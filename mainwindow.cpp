#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "filereader.h"
#include "datamanager.h"
#include "dataviewer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //On instancie la fenêtre
    ui->setupUi(this);

    //On instancie un DataManager et un FileReader
    dataManager = new DataManager();
    fileReader = new FileReader(dataManager);

    //On fait appel au fileReader pour parser les fichiers à l'aide de la fonction parseFile()
    fileReader->setFileName("GVE_Export.xml");
    fileReader->parseFile("Objects_list", "Object", "Id", "mapGVE");
    fileReader->setFileName("GCA_Export.xml");
    fileReader->parseFile("Objects_list", "Object", "Id", "mapGCA");
    fileReader->setFileName("GAT_Export.xml");
    fileReader->parseFile("Objects_list", "Object", "Id", "mapGAT");
    fileReader->setFileName("GRS_Export.xml");
    fileReader->parseFile("Objects_list", "Object", "Id", "mapGRS");
    fileReader->setFileName("GCS_Export.xml");
    fileReader->parseFile("Objects_list", "Object", "CodeObjet", "mapGCS");

    //On rempli la mapConcordance avec les valeurs du fichier
    dataManager->setDataOfMapConcordance();

    //On définit le nom de la configuration courante par défaut
    dataManager->setCurrentConfigName("*standard");

}

//Destructeur
MainWindow::~MainWindow()
{
    delete dataViewer;
    delete dataManager;
    delete fileReader;
    delete ui;
}


void MainWindow::on_environmentalVariablesButton_released()
{
    //On instancie une vue dataViewer en rentrant les valeurs données par le dataManager
    dataViewer = new DataViewer(dataManager->getAttributesOfCurrentConfig("GVE"),dataManager->getSmallMapsFromMapName("mapGVE", "GVE"), this);
    dataViewer->exec();
}

void MainWindow::on_attributesConfigurationsButton_released()
{
    //On instancie une vue dataViewer en rentrant les valeurs données par le dataManager
    dataViewer = new DataViewer(dataManager->getAttributesOfCurrentConfig("GCA"),dataManager->getSmallMapsFromMapName("mapGCA", "GCA"), this);
    dataViewer->exec();
}

void MainWindow::on_attributesButton_released()
{
    //On instancie une vue dataViewer en rentrant les valeurs données par le dataManager
    dataViewer = new DataViewer(dataManager->getAttributesOfCurrentConfig("GAT"),dataManager->getSmallMapsFromMapName("mapGAT", "GAT"), this);
    dataViewer->exec();
}

void MainWindow::on_caseSelectionButton_released()
{

}

void MainWindow::on_officialsButton_released()
{
    //On instancie une vue dataViewer en rentrant les valeurs données par le dataManager
    dataViewer = new DataViewer(dataManager->getAttributesOfCurrentConfig("GRS"),dataManager->getSmallMapsFromMapName("mapGRS", "GRS"), this);
    dataViewer->exec();
}
