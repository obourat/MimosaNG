#include "dataviewer.h"
#include "ui_dataviewer.h"
#include "model.h"
#include "datamanager.h"
#include "optionsviewer.h"
#include "filereader.h"

#include <QSortFilterProxyModel>
#include <QtGui>

//Constructeur
DataViewer::DataViewer(DataManager *dataManager,const QList<QMap<QString, QString> >& maps, const QString codeObject, QWidget *parent) : dataManager(dataManager),
    QDialog(parent),
    ui(new Ui::DataViewer)
{
    //On instancie la fenêtre
    ui->setupUi(this);

    this->codeObject = codeObject;

    //On définit un modèle qui contient toutes les clés récupérées
    myModel = new Model(maps);
    //On définit un proxyModel servant pour le tri (croissant, décroissant, alphabétique, numérique)
    proxyModel = new QSortFilterProxyModel(this);

    //On associe les données du model au proxyModel
    proxyModel->setSourceModel(myModel);
    //On associe la vue au modèle contenu dans le proxyModel
    ui->tableView->setModel(proxyModel);

    //On ajuste la taille des colonnes au contenu
    ui->tableView->resizeColumnsToContents();

    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    //ui->tableView->horizontalHeader()->setStretchLastSection(true);

    //QVBoxLayout *l = new QVBoxLayout(this);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(customMenuRequested(QPoint)));

    QString currentConfigName;

    //On met à jour le nom de la fenêtre en fonction du code objet, et le nom de la configuration courante dans le label associé
    if(this->codeObject == "GCA")
    {
        this->setWindowTitle("Liste des objets de type : Configurations d'Attributs");
        currentConfigName = dataManager->getCurrentConfigNameGCA();
        ui->infoNomConf->setText("Nom de la configuration courante : " + currentConfigName);
    }
    else if(this->codeObject == "GAT")
    {
        this->setWindowTitle("Liste des objets de type : Attributs");
        currentConfigName = dataManager->getCurrentConfigNameGAT();
        ui->infoNomConf->setText("Nom de la configuration courante : " + currentConfigName);
    }
    else if(this->codeObject == "GRS")
    {
        this->setWindowTitle("Liste des objets de type : Responsables");
        currentConfigName = dataManager->getCurrentConfigNameGRS();
        ui->infoNomConf->setText("Nom de la configuration courante : " + currentConfigName);
    }
    else if(this->codeObject == "GVE")
    {
        this->setWindowTitle("Liste des objets de type : Variables d'Evironnement");
        currentConfigName = dataManager->getCurrentConfigNameGVE();
        ui->infoNomConf->setText("Nom de la configuration courante : " + currentConfigName);
    }
    else if(this->codeObject == "GDO")
    {
        this->setWindowTitle("Liste des objets de type : Documents");
        //currentConfigName = dataManager->getCurrentConfigNameGDO();
        //ui->infoNomConf->setText("Nom de la configuration courante : " + currentConfigName);
    }


}

//Destructeur
DataViewer::~DataViewer()
{
    delete ui;
}

void DataViewer::customMenuRequested(QPoint pos)
{
    QModelIndex index = ui->tableView->indexAt(pos);
    //QVariant test = myModel->data(index,0);
    //QString aa = test.toString();

    QMenu *menu = new QMenu(this);
    QAction *changeCurrentConfig = new QAction("Voir les configurations d'attribut du type d'objet", this);
    QAction *changeCurrentConfigAttributes = new QAction("Lister les attributs de la configuration courante", this);
    menu->addAction(changeCurrentConfig);
    menu->addAction(changeCurrentConfigAttributes);
    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));

    connect(changeCurrentConfig, SIGNAL(triggered()), this, SLOT(onChangeCurrentConfigButtonTriggered()));
    connect(changeCurrentConfigAttributes, SIGNAL(triggered()), this, SLOT(onChangeCurrentConfigAttributesButtonTriggered()));

}

void DataViewer::onChangeCurrentConfigButtonTriggered()
{

    //On instancie une vue optionsViewer en rentrant les valeurs données par le dataManager en utilisant les méthodes spécifiques aux options
    optionsViewerCurrentConfig = new OptionsViewer(codeObject, dataManager, dataManager->getSmallMapsFromMapNameOptions("mapGCA", "GCA",codeObject),"configurations", this);
    optionsViewerCurrentConfig->exec();

}

void DataViewer::onChangeCurrentConfigAttributesButtonTriggered()
{
    //On instancie une vue optionsViewer en rentrant les valeurs données par le dataManager en utilisant les méthodes spécifiques aux options
    optionsViewerCurrentConfigAttributes = new OptionsViewer(codeObject, dataManager, dataManager->getSmallMapsFromMapNameOptions("mapGAT","GAT", codeObject),"attributes", this);
    optionsViewerCurrentConfigAttributes->exec();
}

