#include "dataviewer.h"
#include "ui_dataviewer.h"
#include "model.h"
#include "datamanager.h"
#include "optionsviewer.h"
#include "filereader.h"
#include "descriptivecard.h"
#include "searchcard.h"

#include <QSortFilterProxyModel>
#include <QtGui>
#include <QMessageBox>
#include <QProgressBar>

//Constructeur
DataViewer::DataViewer(DataManager *dataManager,const QList<QMap<QString, QString> >& maps, const QString codeObject, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataViewer),
    dataManager(dataManager),
    mySetting(new QSettings("dataviewer.cpp", QSettings::IniFormat))

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

    //On autorise les colonnes a être déplacées
    ui->tableView->horizontalHeader()->setMovable(true);

    //On ajuste la taille des colonnes au contenu
    ui->tableView->resizeColumnsToContents();

    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);


    if(codeObject == "GCA")
    {
        QByteArray myArray = mySetting.value("columnConfigGCA","").toByteArray();
        ui->tableView->horizontalHeader()->restoreState(myArray);
    }
    else if(codeObject == "GAT")
    {
        QByteArray myArray = mySetting.value("columnConfigGAT","").toByteArray();
        ui->tableView->horizontalHeader()->restoreState(myArray);
    }
    else if(codeObject == "GVE")
    {
        QByteArray myArray = mySetting.value("columnConfigGVE","").toByteArray();
        ui->tableView->horizontalHeader()->restoreState(myArray);
    }
    else if(codeObject == "GDO")
    {
        QByteArray myArray = mySetting.value("columnConfigGDO","").toByteArray();
        ui->tableView->horizontalHeader()->restoreState(myArray);
    }
    else if(codeObject == "GRS")
    {
        QByteArray myArray = mySetting.value("columnConfigGRS","").toByteArray();
        ui->tableView->horizontalHeader()->restoreState(myArray);
    }

    connect(ui->tableView->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customHeaderMenuRequested(QPoint)));
    connect(ui->tableView->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(resizeSection()));
    //ui->tableView->horizontalHeader()->setStretchLastSection(true);

    //QVBoxLayout *l = new QVBoxLayout(this);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(customMenuRequested(QPoint)));
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onItemDoubleClicked()));

    QString currentConfigName;

    QAbstractItemModel* tableModel = ui->tableView->model();
    columnCount = tableModel->columnCount();
    rowCount = tableModel->rowCount();


    QString columnName;
    int columnIteratorMax = columnCount;
    for(int i=0; i!= columnIteratorMax; ++i)
    {
        columnName = ui->tableView->model()->headerData(i,Qt::Horizontal).toString();
        if(columnName == "key")
        {
            ui->tableView->setColumnHidden(i,true);
            break;
        }
    }


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
        currentConfigName = dataManager->getCurrentConfigNameGDO();
        ui->infoNomConf->setText("Nom de la configuration courante : " + currentConfigName);
    }

    QString rows = QString::number(rowCount);

    ui->infoNbObject->setText("Nombre d'objets: "+rows);

    rowsDisplayed = rowCount;

    //Permet de minimiser la fenêtre
    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint| Qt::WindowMinimizeButtonHint| Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    QPalette Pal(palette());
    Pal.setColor(QPalette::Window, QColor(255,255,255,240));
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
    this->show();

}

void DataViewer::updateLayout()
{
    QString currentConfigName;
    if(codeObject == "GCA")
    {
        const QList<QMap<QString, QString> > maps = dataManager->getSmallMapsFromMapName("mapGCA", "GCA");
        Model *newModel = new Model(maps);
        proxyModel->setSourceModel(newModel);
        ui->tableView->setModel(proxyModel);
        currentConfigName = dataManager->getCurrentConfigNameGCA();
        ui->infoNomConf->setText("Nom de la configuration courante : " + currentConfigName);
    }
    else if(codeObject == "GAT")
    {
        const QList<QMap<QString, QString> > maps = dataManager->getSmallMapsFromMapName("mapGAT", "GAT");
        Model *newModel = new Model(maps);
        proxyModel->setSourceModel(newModel);
        ui->tableView->setModel(proxyModel);
        currentConfigName = dataManager->getCurrentConfigNameGAT();
        ui->infoNomConf->setText("Nom de la configuration courante : " + currentConfigName);
    }
    else if(codeObject == "GRS")
    {
        const QList<QMap<QString, QString> > maps = dataManager->getSmallMapsFromMapName("mapGRS", "GRS");
        Model *newModel = new Model(maps);
        proxyModel->setSourceModel(newModel);
        ui->tableView->setModel(proxyModel);
        currentConfigName = dataManager->getCurrentConfigNameGRS();
        ui->infoNomConf->setText("Nom de la configuration courante : " + currentConfigName);
    }
    else if(codeObject == "GDO")
    {
        const QList<QMap<QString, QString> > maps = dataManager->getSmallMapsFromMapName("mapGDO", "GDO");
        Model *newModel = new Model(maps);
        proxyModel->setSourceModel(newModel);
        ui->tableView->setModel(proxyModel);
        currentConfigName = dataManager->getCurrentConfigNameGDO();
        ui->infoNomConf->setText("Nom de la configuration courante : " + currentConfigName);
    }
    else if(codeObject == "GVE")
    {
        const QList<QMap<QString, QString> > maps = dataManager->getSmallMapsFromMapName("mapGVE", "GVE");
        Model *newModel = new Model(maps);
        proxyModel->setSourceModel(newModel);
        ui->tableView->setModel(proxyModel);
        currentConfigName = dataManager->getCurrentConfigNameGVE();
        ui->infoNomConf->setText("Nom de la configuration courante : " + currentConfigName);
    }

    QAbstractItemModel* tableModel = ui->tableView->model();
    columnCount = tableModel->columnCount();

    int columnIteratorMax = columnCount;
    QString columnName;
    for(int i=0; i!= columnIteratorMax; ++i)
    {
        columnName = ui->tableView->model()->headerData(i,Qt::Horizontal).toString();
        if(columnName == "key")
        {
            ui->tableView->setColumnHidden(i,true);
            ui->tableView->columnAt(i);
            break;
        }
    }


}


//Destructeur
DataViewer::~DataViewer()
{
    QByteArray myArray = ui->tableView->horizontalHeader()->saveState();
    if(codeObject == "GCA")
    {
        mySetting.setValue("columnConfigGCA", myArray);
    }
    else if(codeObject == "GAT")
    {
        mySetting.setValue("columnConfigGAT", myArray);
    }
    else if(codeObject == "GVE")
    {
        mySetting.setValue("columnConfigGVE", myArray);
    }
    else if(codeObject == "GDO")
    {
        mySetting.setValue("columnConfigGDO", myArray);
    }
    if(codeObject == "GRS")
    {
        mySetting.setValue("columnConfigGRS", myArray);
    }
    delete ui;
}

void DataViewer::customMenuRequested(QPoint pos)
{
    keysList.clear();

    QModelIndex index = ui->tableView->indexAt(pos);
#if 0
    //On compte le nombre de lignes sélectionnées
    int rowCount = ui->tableView->selectionModel()->selectedRows().count();
#endif
    //On compte le nombre de colonnes sélectionnées -1 pour aoir l'index de la dernière colonne
    QAbstractItemModel* tableModel = ui->tableView->model();
    int columnCount = tableModel->columnCount();
    --columnCount;
    //On crée le string permettant de stocker la clé de la ligne sélectionnée
    QString key;
    //On crée la liste d'index qui contient tous les index de toutes les colonnes des lignes sélectionnées
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();

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

    QMenu* menu = new QMenu(this);
    QAction* changeCurrentConfig = new QAction("Voir les configurations d'attribut du type d'objet", this);
    QAction* changeCurrentConfigAttributes = new QAction("Lister les attributs de la configuration courante", this);
    QMenu* selection = menu->addMenu("Selectionner");
    QAction* subList = selection->addAction("Sous-Liste");
    QAction* total = selection->addAction("Totalite");
    QMenu* manually = selection->addMenu("Manuellement");
    QAction* validateSelection = manually->addAction("Valider la selection");
    QAction* hideSelection = manually->addAction("Cacher la selection");
    QAction* resetSelection = menu->addAction("Rafraichir");

    menu->addAction(changeCurrentConfig);
    menu->addAction(changeCurrentConfigAttributes);

    if(keysList.length() == 1)
    {
        QMenu *displayDescriptiveCardMenu = menu->addMenu("Afficher la fiche descriptive de l'objet selectionne");
        QAction* displayDescriptiveCardComplete = displayDescriptiveCardMenu->addAction("Fiche complete");
        QAction* displayDescriptiveCardCurrent = displayDescriptiveCardMenu->addAction("Fiche de la configuration courante");
        connect(displayDescriptiveCardCurrent, SIGNAL(triggered()), this, SLOT(onDisplayDescriptiveCardButtonTriggered()));
        connect(displayDescriptiveCardComplete, SIGNAL(triggered()), this, SLOT(onDisplayDescriptiveCardCompleteButtonTriggered()));
    }

    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));

    connect(changeCurrentConfig, SIGNAL(triggered()), this, SLOT(onChangeCurrentConfigButtonTriggered()));
    connect(changeCurrentConfigAttributes, SIGNAL(triggered()), this, SLOT(onChangeCurrentConfigAttributesButtonTriggered()));
    connect(validateSelection, SIGNAL(triggered()), this, SLOT(onValidateSelectionButtonTriggered()));
    connect(hideSelection, SIGNAL(triggered()), this, SLOT(onHideSelectionButtonTriggered()));
    connect(resetSelection, SIGNAL(triggered()), this, SLOT(onResetSelectionButtonTriggered()));
    connect(total, SIGNAL(triggered()), this, SLOT(onTotalSelectionButtonTriggered()));
    connect(subList, SIGNAL(triggered()), this, SLOT(onSubListButtonTriggered()));

}

void DataViewer::customHeaderMenuRequested(QPoint pos)
{
    index = ui->tableView->horizontalHeader()->logicalIndexAt(pos);

    QMenu* menu = new QMenu(this);
    QAction* hideColumn = new QAction("Masquer la colonne selectionnee", this);
    menu->addAction(hideColumn);
    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    connect(hideColumn, SIGNAL(triggered()), this, SLOT(setColumnHidden()));

}



void DataViewer::onChangeCurrentConfigButtonTriggered()
{
    //On instancie une vue optionsViewer en rentrant les valeurs données par le dataManager en utilisant les méthodes spécifiques aux options
    optionsViewerCurrentConfig = new OptionsViewer(codeObject, dataManager,this,dataManager->getSmallMapsFromMapNameOptions("mapGCA", "GCA",codeObject),"configurations", this);
    optionsViewerCurrentConfig->exec();

}

void DataViewer::onChangeCurrentConfigAttributesButtonTriggered()
{
    //On instancie une vue optionsViewer en rentrant les valeurs données par le dataManager en utilisant les méthodes spécifiques aux options
    optionsViewerCurrentConfigAttributes = new OptionsViewer(codeObject, dataManager,this, dataManager->getSmallMapsFromMapNameOptions("mapGAT","GAT", codeObject),"attributes", this);
    optionsViewerCurrentConfigAttributes->exec();
}

void DataViewer::onDisplayDescriptiveCardButtonTriggered()
{
    //On instancie une vue descriptiveCard correspondant à la fiche descriptive pour l'objjet sélectionné
    descriptiveCard = new DescriptiveCard(dataManager, codeObject, keysList[0],"current",this);
    descriptiveCard->setAttribute(Qt::WA_DeleteOnClose);
    descriptiveCard->show();

}

void DataViewer::onDisplayDescriptiveCardCompleteButtonTriggered()
{
    //On instancie une vue descriptiveCard correspondant à la fiche descriptive pour l'objjet sélectionné
    descriptiveCard = new DescriptiveCard(dataManager, codeObject, keysList[0],"complete",this);
    descriptiveCard->setAttribute(Qt::WA_DeleteOnClose);
    descriptiveCard->show();
}

void DataViewer::onValidateSelectionButtonTriggered()
{
    QString testedKey;
    int columnCountValue = columnCount;
    columnCountValue--;
    int rowCountValue = rowCount;

    for(int k=0; k!= rowCountValue; ++k)
    {
        for(int l=0; l !=keysList.count(); ++l)
        {
            testedKey = ui->tableView->model()->data(ui->tableView->model()->index(k,columnCountValue)).toString();
            if( testedKey == keysList[l])
            {
                ui->tableView->showRow(k);
                break;
            }
            else
            {
                ui->tableView->hideRow(k);
            }

        }
    }
    QString rows = QString::number(keysList.count());
    ui->infoNbObject->setText("Nombre d'objets: "+rows);
}

void DataViewer::onHideSelectionButtonTriggered()
{
    QString testedKey;
    int columnCountValue = columnCount;
    columnCountValue--;
    int rowCountValue = rowCount;

    for(int k=0; k!= rowCountValue; ++k)
    {
        for(int l=0; l !=keysList.count(); ++l)
        {
            testedKey = ui->tableView->model()->data(ui->tableView->model()->index(k,columnCountValue)).toString();
            if( testedKey == keysList[l])
            {
                ui->tableView->hideRow(k);
                break;
            }
        }
    }
    rowsDisplayed = rowsDisplayed - keysList.count();
    QString rows = QString::number(rowsDisplayed);
    ui->infoNbObject->setText("Nombre d'objets: "+rows);
}

void DataViewer::onResetSelectionButtonTriggered()
{
    int rowCountValue = rowCount;

    for(int k=0; k!= rowCountValue; ++k)
    {
        ui->tableView->showRow(k);
    }
//    int columnCountMinus = --columnCount;
//    for(int l=0; l!= columnCountMinus; ++l)
//    {
//        ui->tableView->showColumn(l);
//    }
    QString rows = QString::number(rowCountValue);
    ui->infoNbObject->setText("Nombre d'objets: "+rows);
}

void DataViewer::onTotalSelectionButtonTriggered()
{
    ui->tableView->selectAll();
}

void DataViewer::onSubListButtonTriggered()
{
    searchCard = new SearchCard(dataManager, codeObject, keysList[0],this);
    searchCard->exec();
#warning manque le connecteur a la validation
    //connect(searchCard, SIGNAL(accepted()), this, SLOT(getSearchResults()));

    QList<QString> searchResults = searchCard->getSearchResults();
    int newRowCount = 0;
    QString testedKey;
    int columnCountValue = columnCount;
    columnCountValue--;
    int rowCountValue = rowCount;
    QProgressDialog progress("Affichage des resultats", "Annuler", 0, searchResults.count(), this);
    progress.setWindowModality(Qt::WindowModal);

#warning processus lent

    for(int k=0; k!= rowCountValue; ++k)
    {
        progress.setValue(k);
        if(progress.wasCanceled())
        {
            break;
        }

        for(int l=0; l !=searchResults.count(); ++l)
        {
            testedKey = ui->tableView->model()->data(ui->tableView->model()->index(k,columnCountValue)).toString();
            if( testedKey == searchResults[l])
            {
                ui->tableView->showRow(k);
                ++newRowCount;
                break;
            }
            else
            {
                ui->tableView->hideRow(k);
            }

        }
    }
    progress.setValue(rowCountValue);


#if 0
    for(int j=0; j!= rowCountValue; ++j)
    {
        ui->tableView->hideRow(j);
    }

    for(int l=0; l!= searchResults.count(); ++l)
    {
        progress.setValue(l);
        if(progress.wasCanceled())
        {
            break;
        }

        for(int k=0; k!= rowCountValue; ++k)
        {
            testedKey = ui->tableView->model()->data(ui->tableView->model()->index(k,columnCountValue)).toString();
            if(testedKey == searchResults[l])
            {
                ui->tableView->showRow(k);
                ++newRowCount;
                break;
            }
        }
    }
    progress.setValue(searchResults.count());

#endif

    int confirmsearch = searchCard->getConfirmSearch();

    if(newRowCount == 0 && confirmsearch == 1)
    {
        QMessageBox::information(this, "Information", "Aucun resultat disponible pour cette recherche");
    }

    if(newRowCount != 0)
    {
        QString newRowCountToString = QString::number(newRowCount);
        ui->infoNbObject->setText("Nombre d'objets: "+newRowCountToString);
    }

}


void DataViewer::getSearchResults()
{
    QList<QString> searchResults = searchCard->getSearchResults();
}

void DataViewer::onItemDoubleClicked()
{
    keysList.clear();
    //On crée la liste d'index qui contient tous les index de toutes les colonnes des lignes sélectionnées
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();
    QString key;
    int columnCountValue = columnCount;
    columnCountValue--;
    key = selectedIndexes[columnCountValue].data(0).toString();
    keysList.append(key);
    //On instancie une vue descriptiveCard correspondant à la fiche descriptive pour l'objjet sélectionné
    descriptiveCard = new DescriptiveCard(dataManager, codeObject, keysList[0],"complete",this);
    descriptiveCard->show();
}

void DataViewer::setColumnHidden()
{
    ui->tableView->setColumnHidden(index,true);
}



