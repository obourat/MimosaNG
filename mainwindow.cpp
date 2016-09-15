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

    //On instancie une vue dataViewer en rentrant les valeurs données par le dataManager
    dataViewer = new DataViewer(dataManager->getKeysOfCurrentConfig("GVE"),dataManager->getSmallMapsFromMapName("mapGVE"), this);
    dataViewer->exec();
}

//Destructeur
MainWindow::~MainWindow()
{
    delete dataViewer;
    delete dataManager;
    delete fileReader;
    delete ui;
}
