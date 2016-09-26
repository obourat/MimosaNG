#include "optionsviewer.h"
#include "datamanager.h"
#include "descriptivecard.h"
#include "ui_optionsviewer.h"
#include "model.h"


#include <QSortFilterProxyModel>
#include <QtGui>

OptionsViewer::OptionsViewer(QString codeObject, DataManager *dataManager, const QList<QMap<QString, QString> >& maps, QString selectedOption, QWidget *parent) : dataViewer(dataViewer), codeObject(codeObject), dataManager(dataManager), selectedOption(selectedOption),
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

    ui->optionsView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->optionsView, SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(customMenuRequested(QPoint)));

    if(this->selectedOption == "configurations")
    {
        this->setWindowTitle("Liste des configurations d'attributs");
        ui->instructionsLabel->setText("Selectionnez une configuration dans la liste et cliquez sur OK pour la choisir en tant que configuration courante");
    }
    else if(this->selectedOption == "attributes")
    {
        QString currentConfigName;
        this->setWindowTitle("Liste des attributs de la configuration courante");

        if(this->codeObject == "GCA")
        {
            currentConfigName = dataManager->getCurrentConfigNameGCA();
        }
        else if(this->codeObject == "GAT")
        {
            currentConfigName = dataManager->getCurrentConfigNameGAT();
        }
        else if(this->codeObject == "GRS")
        {
            currentConfigName = dataManager->getCurrentConfigNameGRS();
        }
        else if(this->codeObject == "GVE")
        {
            currentConfigName = dataManager->getCurrentConfigNameGVE();
        }
        ui->infoNomConf->setText("Nom de la configuration courante : " + currentConfigName);
        ui->instructionsLabel->setText("Selectionnez un attribut dans la liste et cliquez sur OK pour modifier ses caracteristiques");
    }
}

OptionsViewer::~OptionsViewer()
{
    delete ui;
}

void OptionsViewer::customMenuRequested(QPoint pos)
{
    QModelIndex index = ui->optionsView->indexAt(pos);
    QString data = index.data(0).toString();


    QMenu *menu = new QMenu(this);
    QAction *popObjectCard = new QAction("Voir la fiche descriptive de l'objet", this);
    menu->addAction(popObjectCard);
    menu->popup(ui->optionsView->viewport()->mapToGlobal(pos));
    connect(popObjectCard, SIGNAL(triggered()), this, SLOT(onPopObjectCardButtonTriggered()));
}

void OptionsViewer::on_confirmButtonBox_accepted()
{
    if(selectedOption == "configurations")
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
    }

    else if(selectedOption == "attributes")
    {
        descriptiveCard = new DescriptiveCard(this);
        descriptiveCard->exec();
    }

    //Model model = ui->optionsView->model();
    //QString data;
}



void OptionsViewer::on_optionsView_clicked()
{
    currentConfigSelectedName = ui->optionsView->selectionModel()->currentIndex().data(0).toString();
}

void OptionsViewer::onPopObjectCardButtonTriggered()
{
    descriptiveCard = new DescriptiveCard(this);
    descriptiveCard->exec();
}
