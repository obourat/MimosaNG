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

    //On autorise les colonnes a être déplacées
    ui->optionsView->horizontalHeader()->setMovable(true);

    //On ajuste la taille des colonnes au contenu
    ui->optionsView->resizeColumnsToContents();

    ui->optionsView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    //ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui->optionsView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->optionsView, SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(customMenuRequested(QPoint)));

    QAbstractItemModel* tableModel = ui->optionsView->model();
    int columnCount = tableModel->columnCount();
    int rowCount = tableModel->rowCount();
    ui->optionsView->setColumnHidden(--columnCount, true);

    QString rows = QString::number(rowCount);

    ui->infoNbObject->setText("Nombre d'objets: "+rows);

    if(this->selectedOption == "configurations")
    {
        this->setWindowTitle("Liste des configurations d'attributs");
        ui->instructionsLabel->setText("Selectionnez une configuration dans la liste et cliquez sur OK pour la choisir en tant que configuration courante");

        if(this->codeObject == "GCA")
        {
            ui->infoNomConf->setText("Liste des configurations d'attributs disponibles pour le type d'objet : Configuration d'Attribut");
        }
        else if(this->codeObject == "GAT")
        {
            ui->infoNomConf->setText("Liste des configurations d'attributs disponibles pour le type d'objet : Attribut");
        }
        else if(this->codeObject == "GRS")
        {
            ui->infoNomConf->setText("Liste des configurations d'attributs disponibles pour le type d'objet : Responsable");
        }
        if(this->codeObject == "GVE")
        {
            ui->infoNomConf->setText("Liste des configurations d'attributs disponibles pour le type d'objet : Variable d'Environnement");
        }
    }
    else if(this->selectedOption == "attributes")
    {
        QString currentConfigName;
        this->setWindowTitle("Liste des attributs de la configuration courante");

        if(this->codeObject == "GCA")
        {
            currentConfigName = dataManager->getCurrentConfigNameGCA();
            ui->infoNomConf->setText("Nom de la configuration courante : \"" + currentConfigName + "\"  pour le type d'objet : Configuration d'Attribut");
        }
        else if(this->codeObject == "GAT")
        {
            currentConfigName = dataManager->getCurrentConfigNameGAT();
            ui->infoNomConf->setText("Nom de la configuration courante : \"" + currentConfigName + "\"  pour le type d'objet : Attribut");
        }
        else if(this->codeObject == "GRS")
        {
            currentConfigName = dataManager->getCurrentConfigNameGRS();
            ui->infoNomConf->setText("Nom de la configuration courante : \"" + currentConfigName + "\"  pour le type d'objet : Responsable");
        }
        else if(this->codeObject == "GVE")
        {
            currentConfigName = dataManager->getCurrentConfigNameGVE();
            ui->infoNomConf->setText("Nom de la configuration courante : \"" + currentConfigName + "\"  pour le type d'objet : Variable d'Environnement");
        }
        ui->instructionsLabel->setText("Selectionnez un attribut dans la liste et cliquez sur OK pour modifier ses caracteristiques");
    }

    connect(this, SIGNAL(destroyed()), this->parent(), SLOT(close()));
}

OptionsViewer::~OptionsViewer()
{
    delete ui;
}

void OptionsViewer::customMenuRequested(QPoint pos)
{
    keysList.clear();

    QModelIndex index = ui->optionsView->indexAt(pos);
    QString data = index.data(0).toString();

    //On compte le nombre de colonnes sélectionnées -1 pour aoir l'index de la dernière colonne
    QAbstractItemModel* tableModel = ui->optionsView->model();
    int columnCount = tableModel->columnCount();
    --columnCount;
    //On crée le string permettant de stocker la clé de la ligne sélectionnée
    QString key;
    QModelIndexList selectedIndexes = ui->optionsView->selectionModel()->selectedIndexes();

    //On soustrait de la liste d'index les index dont la colonne n'est pas égal à la dernière
    for(int j=0; j<selectedIndexes.count(); ++j)
    {
        if(selectedIndexes[j].column() != columnCount)
        {
            selectedIndexes.removeOne(selectedIndexes[j]);
            --j;
        }
    }

    //Pour les éléments restants de la liste, on va chercher la valeur correpondant à la clé et on l'ajoute à la liste de clés
    for(int i=0; i<selectedIndexes.length(); ++i)
    {
        key = selectedIndexes[i].data(0).toString();
        keysList.append(key);
    }

    QMenu *menu = new QMenu(this);
    QMenu *displayDescriptiveCardMenu = menu->addMenu("Afficher la fiche descriptive de l'objet selectionne");
    QAction* displayDescriptiveCardComplete = displayDescriptiveCardMenu->addAction("Fiche complete");
    QAction* displayDescriptiveCardCurrent = displayDescriptiveCardMenu->addAction("Fiche de la configuration courante");
    connect(displayDescriptiveCardCurrent, SIGNAL(triggered()), this, SLOT(onDisplayDescriptiveCardButtonTriggered()));
    connect(displayDescriptiveCardComplete, SIGNAL(triggered()), this, SLOT(onDisplayDescriptiveCardCompleteButtonTriggered()));

    menu->popup(ui->optionsView->viewport()->mapToGlobal(pos));
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
        //Gérer le cas ou l'on sélectionne ok pour un attribut dans la liste

        //descriptiveCard = new DescriptiveCard(keysList[0],this);
        //descriptiveCard->exec();
    }

    this->~OptionsViewer();

    //Model model = ui->optionsView->model();
    //QString data;
}



void OptionsViewer::on_optionsView_clicked()
{
    //Si il s'agit d'une selecton de configuration, on selectionne la valeur de la première colonne de la selection qui correspond au nom de la conf
    if(selectedOption == "configurations")
    {
        currentConfigSelectedName = ui->optionsView->selectionModel()->selectedIndexes().first().data(0).toString();
    }
}

void OptionsViewer::onDisplayDescriptiveCardButtonTriggered()
{
    if(selectedOption == "configurations")
    {
        //On instancie une vue descriptiveCard correspondant à la fiche descriptive pour l'objet sélectionné
        descriptiveCard = new DescriptiveCard(dataManager, "GCA", keysList[0],"current",this);
        descriptiveCard->show();
    }
    else if(selectedOption == "attributes")
    {
        //On instancie une vue descriptiveCard correspondant à la fiche descriptive pour l'objet sélectionné
        descriptiveCard = new DescriptiveCard(dataManager, "GAT", keysList[0],"current",this);
        descriptiveCard->show();
    }
}

void OptionsViewer::onDisplayDescriptiveCardCompleteButtonTriggered()
{
    if(selectedOption == "configurations")
    {
        //On instancie une vue descriptiveCard correspondant à la fiche descriptive pour l'objet sélectionné
        descriptiveCard = new DescriptiveCard(dataManager, "GCA", keysList[0],"complete",this);
        descriptiveCard->show();
    }
    else if(selectedOption == "attributes")
    {
        //On instancie une vue descriptiveCard correspondant à la fiche descriptive pour l'objet sélectionné
        descriptiveCard = new DescriptiveCard(dataManager, "GAT", keysList[0],"complete",this);
        descriptiveCard->show();
    }
}
