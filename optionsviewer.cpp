#include "optionsviewer.h"
#include "datamanager.h"
#include "ui_optionsviewer.h"
#include "model.h"


#include <QSortFilterProxyModel>

OptionsViewer::OptionsViewer(QString codeObject, DataManager *dataManager, const QList<QMap<QString, QString> >& maps, QWidget *parent) : codeObject(codeObject), dataManager(dataManager),
    QDialog(parent),
    ui(new Ui::OptionsViewer)
{
    ui->setupUi(this);

    //On définit un modèle qui contient toutes les clés récupérées
    myOptionsModel = new Model(maps);
    //On définit un proxyModel servant pour le tri (croissant, décroissant, alphabétique, numérique)
    proxyOptionsModel = new QSortFilterProxyModel(this);

    //On associe les données du model au proxyModel
    proxyOptionsModel->setSourceModel(myOptionsModel);
    //On associe la vue au modèle contenu dans le proxyModel
    ui->optionsView->setModel(proxyOptionsModel);

    //On ajuste la taille des colonnes au contenu
    ui->optionsView->resizeColumnsToContents();

    ui->optionsView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    //ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

OptionsViewer::~OptionsViewer()
{
    delete ui;
}

void OptionsViewer::on_confirmButtonBox_accepted()
{
    if(!currentConfigSelectedName.isNull())
    {
        if(codeObject == "GCA")
        {
            dataManager->setCurrentConfigNameGCA(currentConfigSelectedName);
        }
        else if(codeObject == "GAT")
        {
            dataManager->setCurrentConfigNameGAT(currentConfigSelectedName);
        }
        else if(codeObject == "GRS")
        {
            dataManager->setCurrentConfigNameGRS(currentConfigSelectedName);
        }
        else if(codeObject == "GVE")
        {
            dataManager->setCurrentConfigNameGVE(currentConfigSelectedName);
        }

    }

    //Model model = ui->optionsView->model();
    //QString data;
}



void OptionsViewer::on_optionsView_clicked()
{
    currentConfigSelectedName = ui->optionsView->selectionModel()->currentIndex().data(0).toString();
}
