#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "filereader.h"
#include "datamanager.h"
#include "dataviewer.h"
#include "filewriter.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //On instancie la fenêtre
    ui->setupUi(this);

    //On instancie un DataManager et un FileReader, ainsi qu'un FileWriter
    dataManager = new DataManager();
    fileReader = new FileReader(dataManager);
    fileWriter = new FileWriter(dataManager);
    QString standardConfigName;

    //On fait appel au fileReader pour parser les fichiers à l'aide de la fonction parseFile()
    fileReader->setFileName("GVE_Export.xml");
    fileReader->parseFile("Objects_list", "Object", "Id", "mapGVE");
    fileReader->setFileName("GCA_Export.xml");
    fileReader->parseFile("Objects_list", "Object", "Id", "mapGCA");
    fileReader->setFileName("GAT_Export.xml");
    fileReader->parseFile("Objects_list", "Object", "Id", "mapGAT");
    fileReader->setFileName("GRS_Export.xml");
    fileReader->parseFile("Objects_list", "Object", "Id", "mapGRS");
    fileReader->setFileName("GDO_Export.xml");
    fileReader->parseFile("Objects_list", "Object", "NumOrdre", "mapGDO");
    fileReader->setFileName("GCS_Export.xml");
    fileReader->parseFile("Objects_list", "Object", "CodeObjet", "mapGCS");

    //On rempli la mapConcordance avec les valeurs du fichier
    dataManager->setDataOfMapConcordance();

    //On définit le nom de la configuration courante par défaut
    standardConfigName = dataManager->getStandardConfigName("GCA");
    dataManager->setCurrentConfigNameGCA(standardConfigName);
    standardConfigName = dataManager->getStandardConfigName("GAT");
    dataManager->setCurrentConfigNameGAT(standardConfigName);
    standardConfigName = dataManager->getStandardConfigName("GRS");
    dataManager->setCurrentConfigNameGRS(standardConfigName);
    standardConfigName = dataManager->getStandardConfigName("GVE");
    dataManager->setCurrentConfigNameGVE(standardConfigName);
    standardConfigName = dataManager->getStandardConfigName("GDO");
    dataManager->setCurrentConfigNameGDO(standardConfigName);

    QPalette Pal(palette());
    Pal.setColor(QPalette::Window, QColor(255,255,255,240));
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
    this->show();

}

//Destructeur
MainWindow::~MainWindow()
{
    //delete dataViewer;
    fileWriter->modifyFiles("mapGCS");
    delete dataManager;
    delete fileReader;
    delete ui;
}


void MainWindow::on_environmentalVariablesButton_released()
{
    //On instancie une vue dataViewer en rentrant les valeurs données par le dataManager
    dataViewer = new DataViewer(dataManager, dataManager->getSmallMapsFromMapName("mapGVE", "GVE"),"GVE", this);
    dataViewer->setAttribute(Qt::WA_DeleteOnClose);
    dataViewer->show();

}

void MainWindow::on_attributesConfigurationsButton_released()
{
    //On instancie une vue dataViewer en rentrant les valeurs données par le dataManager
    dataViewer = new DataViewer(dataManager, dataManager->getSmallMapsFromMapName("mapGCA", "GCA"), "GCA",  this);
    dataViewer->setAttribute(Qt::WA_DeleteOnClose);
    dataViewer->show();
}

void MainWindow::on_attributesButton_released()
{
    //On instancie une vue dataViewer en rentrant les valeurs données par le dataManager
    dataViewer = new DataViewer(dataManager, dataManager->getSmallMapsFromMapName("mapGAT", "GAT"), "GAT", this);
    dataViewer->setAttribute(Qt::WA_DeleteOnClose);
    dataViewer->show();
}

void MainWindow::on_caseSelectionButton_released()
{

}

void MainWindow::on_officialsButton_released()
{
    //On instancie une vue dataViewer en rentrant les valeurs données par le dataManager
    dataViewer = new DataViewer(dataManager, dataManager->getSmallMapsFromMapName("mapGRS", "GRS"), "GRS", this);
    dataViewer->setAttribute(Qt::WA_DeleteOnClose);
    dataViewer->show();
}

void MainWindow::on_documentsButton_released()
{
    //On instancie une vue dataViewer en rentrant les valeurs données par le dataManager
    dataViewer = new DataViewer(dataManager, dataManager->getSmallMapsFromMapName("mapGDO", "GDO"), "GDO", this);
    dataViewer->setAttribute(Qt::WA_DeleteOnClose);
    dataViewer->show();
}
