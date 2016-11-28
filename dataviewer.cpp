#include "dataviewer.h"
#include "ui_dataviewer.h"
#include "model.h"
#include "datamanager.h"
#include "optionsviewer.h"
#include "filereader.h"
#include "descriptivecard.h"
#include "searchcard.h"
#include "mainwindow.h"
#include "exportform.h"
#include "printform.h"
#include "importform.h"

#include <QSortFilterProxyModel>
#include <QtGui>
#include <QMessageBox>
#include <QProgressBar>
#include <QString>

//Constructeur
DataViewer::DataViewer(DataManager *dataManager, MainWindow *mainWindow, const QList<QMap<QString, QString> >& maps, const QString codeObject, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataViewer),
    dataManager(dataManager),
    mainWindow(mainWindow),
    codeObject(codeObject),
    mySetting(new QSettings("dataviewer.cpp", QSettings::IniFormat))

{
    //On instancie la fenêtre
    ui->setupUi(this);

    //On définit un modèle qui contient toutes les clés récupérées
    myModel = new Model(maps);
    //On définit un proxyModel servant pour le tri (croissant, décroissant, alphabétique, numérique)
    proxyModel = new QSortFilterProxyModel(this);

    //On associe les données du model au proxyModel
    proxyModel->setSourceModel(myModel);
    proxyModel->setDynamicSortFilter(true);

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
    connect(ui->tableView->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onSortContent()));
    //ui->tableView->horizontalHeader()->setStretchLastSection(true);

    //QVBoxLayout *l = new QVBoxLayout(this);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(customMenuRequested(QPoint)));
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onItemDoubleClicked()));

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

    //Insertion du couple clé, numéro de ligne pour chaque ligne dans la map keyRowMap
    updateKeyRowMap();

    //Permet de minimiser la fenêtre
    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint| Qt::WindowMinimizeButtonHint| Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    QPalette pal(palette());
    QLinearGradient gradient(this->rect().topLeft(), this->rect().bottomRight());
    gradient.setColorAt(0, QColor(255,255,255,255));
    gradient.setColorAt(1, QColor(245,255,255,255));
    pal.setBrush(QPalette::Background, QBrush(gradient));
    this->setPalette(pal);

    //On initalise l'indicateur de première recherche à 1
    indicFirstSearch = 1;

    //On initialise le choix d'ajout d'objet à none
    mainWindow->setChoiceAddObject("none");

    //Quand le signal update est emis dans mainWindow, on update le modele
    connect(mainWindow, SIGNAL(signalUpdateLayoutsViewers()), this, SLOT(slotUpdateLayout()));

    //Quand le signal removeColumn est emis dans mainWindow, on update le modele
    connect(mainWindow, SIGNAL(signalChangeColumn()), this, SLOT(slotChangeColumn()));

}

void DataViewer::updateLayout()
{
    QString currentConfigName;
    QString idLastKeyAdded = dataManager->getIdOfLastCreatedObject();
    QStringList idLastSupprObjects = dataManager->getIdOfLastSupprObjects();
    QStringList keysToTreat = mainWindow->getKeysToTreat();
    QString choiceAddObject = mainWindow->getChoiceAddObject();
    QString testedKey;
    int columnIteratorMax = columnCount;
    int columnOfKey;
    QString columnName;
    int rows = rowCount;
    int indicRestoreState = dataManager->getIndicRestoreState();
    displayedRowsBeforeUpdate.clear();

    for(int i=0; i!= columnIteratorMax; ++i)
    {
        //Quand on change de configuration, on réaffiche toutes les colonnes que l'on a pu supprimer de l'affichage
        if(indicRestoreState == 0)
        {
           ui->tableView->setColumnHidden(i, false);
        }
    }
    dataManager->setIndicRestoreState(1);

    //On cherche le numéro de colonne correspondant à la clé
    for(int j=0; j!= columnIteratorMax; ++j)
    {
        columnName = ui->tableView->model()->headerData(j,Qt::Horizontal).toString();
        if(columnName == "key")
        {
            columnOfKey = j;
            break;
        }
    }
    //On ajoute à la liste displayedRowsBeforeUpdate les clés des lignes affichées avant la mise à jour lors de la création d'un nouvel objet (Utile dans recherche + changement de configuration)
    for(int a=0; a<rows;++a)
    {
        if(ui->tableView->isRowHidden(a) == false)
        {
            testedKey = ui->tableView->model()->data(ui->tableView->model()->index(a,columnOfKey)).toString();
            displayedRowsBeforeUpdate.append(testedKey);
        }
    }

    //On ajoute la nouvelle clé à la ligne pour qu'elle s'affiche dans la recherche actuelle, dans le cas ou on en ajoute une
    if(choiceAddObject == "copy")
    {
        displayedRowsBeforeUpdate.append(idLastKeyAdded);
    }
    //On supprime les clés des clés à afficher dans le cas ou on en a supprimé une ou plusieurs
    if(choiceAddObject == "suppr")
    {
        for(int i=0; i< idLastSupprObjects.count();++i)
        {
            displayedRowsBeforeUpdate.removeAll(idLastSupprObjects[i]);
        }
    }

    //Mise à jour du modèle de données
    if(codeObject == "GCA")
    {
        const QList<QMap<QString, QString> > maps = dataManager->getSmallMapsFromMapName("mapGCA", "GCA");
        myModel->updateModelRows(maps,keysToTreat,choiceAddObject,columnOfKey);
        currentConfigName = dataManager->getCurrentConfigNameGCA();
        ui->infoNomConf->setText("Nom de la configuration courante : " + currentConfigName);        
    }
    else if(codeObject == "GAT")
    {
        const QList<QMap<QString, QString> > maps = dataManager->getSmallMapsFromMapName("mapGAT", "GAT");
        myModel->updateModelRows(maps,keysToTreat,choiceAddObject,columnOfKey);
        currentConfigName = dataManager->getCurrentConfigNameGAT();
        ui->infoNomConf->setText("Nom de la configuration courante : " + currentConfigName);
    }
    else if(codeObject == "GRS")
    {
        const QList<QMap<QString, QString> > maps = dataManager->getSmallMapsFromMapName("mapGRS", "GRS");
        myModel->updateModelRows(maps,keysToTreat,choiceAddObject,columnOfKey);
        currentConfigName = dataManager->getCurrentConfigNameGRS();
        ui->infoNomConf->setText("Nom de la configuration courante : " + currentConfigName);
    }
    else if(codeObject == "GDO")
    {
        const QList<QMap<QString, QString> > maps = dataManager->getSmallMapsFromMapName("mapGDO", "GDO");
        myModel->updateModelRows(maps,keysToTreat,choiceAddObject,columnOfKey);
        currentConfigName = dataManager->getCurrentConfigNameGDO();
        ui->infoNomConf->setText("Nom de la configuration courante : " + currentConfigName);
    }
    else if(codeObject == "GVE")
    {
        const QList<QMap<QString, QString> > maps = dataManager->getSmallMapsFromMapName("mapGVE", "GVE");
        myModel->updateModelRows(maps,keysToTreat,choiceAddObject,columnOfKey);
        currentConfigName = dataManager->getCurrentConfigNameGVE();
        ui->infoNomConf->setText("Nom de la configuration courante : " + currentConfigName);
    }
    //On remet à jour les compteurs de lignes et colonnes
    rowCount = ui->tableView->model()->rowCount();
    columnCount = ui->tableView->model()->columnCount();
    columnIteratorMax = columnCount;
    rows = rowCount;

    //On met à jour la map des clés/lignes car il y a eu des modifications (ajout, suppression)
    updateKeyRowMap();

    QString rowsStr = QString::number(rowCount);
    ui->infoNbObject->setText("Nombre d'objets: "+rowsStr);

    // Dans le cas ou on a effectué une recherche, on cache les lignes précédemment cachés en affichant les autres dont la nouvelle
    if(indicFirstSearch == 0)
    {
        for(int i=0; i<rows; ++i)
        {
            ui->tableView->hideRow(i);
        }

        QStringList displayedRows = displayedRowsBeforeUpdate;
        for(int j=0; j<displayedRows.count(); ++j)
        {
            QString row = keyRowMap[displayedRows[j]];
            int rowInt = row.toInt();
            ui->tableView->showRow(rowInt);
        }

        rowsStr = QString::number(displayedRows.count());
        ui->infoNbObject->setText("Nombre d'objets: "+rowsStr);
    }


    //On chache la dernière colonne
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

void DataViewer::updateKeyRowMap()
{
    int columnOfKey;
    int columnIteratorMax = columnCount;
    QString columnName;
    //On cherche le numéro de colonne correspondant à la clé
    for(int i=0; i!= columnIteratorMax; ++i)
    {
        columnName = ui->tableView->model()->headerData(i,Qt::Horizontal).toString();
        if(columnName == "key")
        {
            columnOfKey = i;
            break;
        }
    }

    keyRowMap.clear();
    int rowCountNumber = rowCount;
    QString key;
    QString rowNumber;
    for(int i=0; i!= rowCountNumber; ++i)
    {
        key = ui->tableView->model()->data(ui->tableView->model()->index(i,columnOfKey)).toString();
        rowNumber = QString::number(i);
        keyRowMap.insert(key,rowNumber);
    }
}


//Destructeur
DataViewer::~DataViewer()
{
    //ui->tableView->setModel(myModel);
    QByteArray myArray = ui->tableView->horizontalHeader()->saveState();
    if(codeObject == "GCA")
    {
        myArray.clear();
        mySetting.setValue("columnConfigGCA", myArray);
    }
    else if(codeObject == "GAT")
    {
        myArray.clear();
        mySetting.setValue("columnConfigGAT", myArray);
    }
    else if(codeObject == "GVE")
    {
        myArray.clear();
        mySetting.setValue("columnConfigGVE", myArray);
    }
    else if(codeObject == "GDO")
    {
        myArray.clear();
        mySetting.setValue("columnConfigGDO", myArray);
    }
    if(codeObject == "GRS")
    {
        myArray.clear();
        mySetting.setValue("columnConfigGRS", myArray);
    }
    delete ui;
}

void DataViewer::customMenuRequested(QPoint pos)
{
    keysList.clear();

    int accessLevel = dataManager->getAccessLevel();

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
    QAction* changeCurrentConfigAttributes = new QAction("Gerer les attributs de la configuration courante", this);
    QMenu* selection = menu->addMenu("Selectionner");
    QMenu* subList = selection->addMenu("Sous-Liste");
    QAction* total = selection->addAction("Totalite");
    QMenu* manually = selection->addMenu("Manuellement");
    QAction* restrain = subList->addAction("Restreindre");
    QAction* add = subList->addAction("Enrichir");
    QAction* validateSelection = manually->addAction("Valider la selection");
    QAction* hideSelection = manually->addAction("Cacher la selection");
    QAction* resetSelection = menu->addAction("Rafraichir");
    QMenu* create = menu->addMenu("Creer");
    QAction* createNew = create->addAction("A partir d'une fiche vierge");
    QAction* erase = menu->addAction("Supprimer");
    QAction* exportation = menu->addAction("Exporter");
    QAction* print = menu->addAction("Imprimer");
    QAction* importation = menu->addAction("Importer");

    if(codeObject == "GAT")
    {
        QAction* copy = menu->addAction("Copier");
        connect(copy, SIGNAL(triggered()), this, SLOT(onCopyButtonTriggered()));
        if(accessLevel <1)
        {
            copy->setEnabled(false);
        }
    }
    menu->addAction(changeCurrentConfig);
    menu->addAction(changeCurrentConfigAttributes);

    if(keysList.length() == 1)
    {
        QMenu *displayDescriptiveCardMenu = menu->addMenu("Afficher la fiche descriptive de l'objet selectionne");
        QAction* displayDescriptiveCardComplete = displayDescriptiveCardMenu->addAction("Fiche complete");
        QAction* displayDescriptiveCardCurrent = displayDescriptiveCardMenu->addAction("Fiche de la configuration courante");
        QAction* createCopy = create->addAction("Par copie");
        connect(displayDescriptiveCardCurrent, SIGNAL(triggered()), this, SLOT(onDisplayDescriptiveCardButtonTriggered()));
        connect(displayDescriptiveCardComplete, SIGNAL(triggered()), this, SLOT(onDisplayDescriptiveCardCompleteButtonTriggered()));
        connect(createCopy, SIGNAL(triggered()), this, SLOT(onCreateCopyButtonTriggered()));
    }

    if(accessLevel <2)
    {
        createNew->setEnabled(false);
        erase->setEnabled(false);
        create->setEnabled(false);
    }


    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));

    connect(changeCurrentConfig, SIGNAL(triggered()), this, SLOT(onChangeCurrentConfigButtonTriggered()));
    connect(changeCurrentConfigAttributes, SIGNAL(triggered()), this, SLOT(onChangeCurrentConfigAttributesButtonTriggered()));
    connect(validateSelection, SIGNAL(triggered()), this, SLOT(onValidateSelectionButtonTriggered()));
    connect(hideSelection, SIGNAL(triggered()), this, SLOT(onHideSelectionButtonTriggered()));
    connect(resetSelection, SIGNAL(triggered()), this, SLOT(onResetSelectionButtonTriggered()));
    connect(total, SIGNAL(triggered()), this, SLOT(onTotalSelectionButtonTriggered()));
    connect(restrain, SIGNAL(triggered()), this, SLOT(onSubListRestrainButtonTriggered()));
    connect(add, SIGNAL(triggered()), this, SLOT(onSubListAddButtonTriggered()));
    connect(createNew, SIGNAL(triggered()), this, SLOT(onCreateNewButtonTriggered()));
    connect(erase, SIGNAL(triggered()), this, SLOT(onEraseButtonTriggered()));
    connect(exportation, SIGNAL(triggered()), this, SLOT(onExportButtonTriggered()));
    connect(print, SIGNAL(triggered()), this, SLOT(onPrintButtonTriggered()));
    connect(importation, SIGNAL(triggered()), this, SLOT(onImportButtonTriggered()));

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
    optionsViewerCurrentConfig = new OptionsViewer(codeObject, dataManager, mainWindow, this,dataManager->getSmallMapsFromMapNameOptions("mapGCA", "GCA",codeObject),"configurations", this);
    optionsViewerCurrentConfig->setAttribute(Qt::WA_DeleteOnClose);
    optionsViewerCurrentConfig->show();

}

void DataViewer::onChangeCurrentConfigAttributesButtonTriggered()
{
    //On instancie une vue optionsViewer en rentrant les valeurs données par le dataManager en utilisant les méthodes spécifiques aux options
    optionsViewerCurrentConfigAttributes = new OptionsViewer(codeObject, dataManager, mainWindow, this, dataManager->getSmallMapsFromMapNameOptions("mapGAT","GAT", codeObject),"attributes", this);
    optionsViewerCurrentConfigAttributes->setAttribute(Qt::WA_DeleteOnClose);
    optionsViewerCurrentConfigAttributes->show();
}

void DataViewer::onDisplayDescriptiveCardButtonTriggered()
{
    //On instancie une vue descriptiveCard correspondant à la fiche descriptive pour l'objjet sélectionné
    descriptiveCard = new DescriptiveCard(dataManager, mainWindow, this, codeObject, keysList[0],"current","modify",this);
    descriptiveCard->setAttribute(Qt::WA_DeleteOnClose);
    descriptiveCard->show();

}

void DataViewer::onDisplayDescriptiveCardCompleteButtonTriggered()
{
    //On instancie une vue descriptiveCard correspondant à la fiche descriptive pour l'objjet sélectionné
    descriptiveCard = new DescriptiveCard(dataManager, mainWindow, this, codeObject, keysList[0],"complete","modify",this);
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
        for(int l=0; l!=keysList.count(); ++l)
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

    QString rows = QString::number(rowCountValue);
    ui->infoNbObject->setText("Nombre d'objets: "+rows);

    indicFirstSearch = 1;

    resultList.clear();
    //updateLayout();
}

void DataViewer::onTotalSelectionButtonTriggered()
{
    ui->tableView->selectAll();
}

void DataViewer::onSubListRestrainButtonTriggered()
{
    indicSearch = "restrain";

    searchCard = new SearchCard(dataManager,this, codeObject, keysList[0],this);
    searchCard->exec();

    QList<QString> searchResults = searchCard->getSearchResults();
    int newRowCount = 0;
    QString testedKey;
    QString rowValue;
    int rowValueInt;
    int columnCountValue = columnCount;
    columnCountValue--;
    QAbstractItemModel* tableModel = ui->tableView->model();
    rowCount = tableModel->rowCount();
    int rowCountValue = rowCount;


    //Affiche les lignes de résultat obtenus lors de la recherche
    int confirmsearch = searchCard->getConfirmSearch();

    if(confirmsearch == 1)
    {
        QProgressDialog progress("Affichage des resultats", "Annuler", 0, searchResults.count(), this);
        progress.setWindowModality(Qt::WindowModal);

        for(int j=0; j!= rowCountValue; ++j)
        {
            ui->tableView->hideRow(j);
        }

        if(!searchResults.isEmpty())
        {
            for(int i=0; i != searchResults.count(); ++i )
            {
                progress.setValue(i);
                if(progress.wasCanceled())
                {
                    break;
                }

                testedKey = searchResults[i];
                rowValue = keyRowMap.value(testedKey);
                rowValueInt = rowValue.toInt();
                ui->tableView->showRow(rowValueInt);
                ++newRowCount;
            }
        }
        progress.setValue(searchResults.count());
    }

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

void DataViewer::onSubListAddButtonTriggered()
{
    indicSearch = "add";

    searchCard = new SearchCard(dataManager,this, codeObject, keysList[0],this);
    searchCard->exec();

    QList<QString> searchResults = searchCard->getSearchResults();
    int newRowCount = 0;
    QString testedKey;
    QString rowValue;
    int rowValueInt;
    int columnCountValue = columnCount;
    columnCountValue--;
    int rowCountValue = rowCount;


    //Affiche les lignes de résultat obtenus lors de la recherche
    int confirmsearch = searchCard->getConfirmSearch();

    if(confirmsearch == 1)
    {
        QProgressDialog progress("Affichage des resultats", "Annuler", 0, searchResults.count(), this);
        progress.setWindowModality(Qt::WindowModal);

        for(int j=0; j!= rowCountValue; ++j)
        {
            ui->tableView->hideRow(j);
        }

        if(!searchResults.isEmpty())
        {
            for(int i=0; i != searchResults.count(); ++i )
            {
                progress.setValue(i);
                if(progress.wasCanceled())
                {
                    break;
                }

                testedKey = searchResults[i];
                rowValue = keyRowMap.value(testedKey);
                rowValueInt = rowValue.toInt();
                ui->tableView->showRow(rowValueInt);
                ++newRowCount;
            }
        }

        progress.setValue(searchResults.count());
    }

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


void DataViewer::onItemDoubleClicked()
{
    keysList.clear();
    //On crée la liste d'index qui contient tous les index de toutes les colonnes des lignes sélectionnées
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();
    QString key;
    QString columnName;
    int columnOfKey;

    //On cherche le numéro de colonne de la clé dans le tableau
    for(int j=0; j<ui->tableView->model()->columnCount();++j)
    {
        columnName = ui->tableView->model()->headerData(j,Qt::Horizontal).toString();
        if(columnName == "key")
        {
            columnOfKey = j;
            break;
        }
    }
    key = selectedIndexes[columnOfKey].data(0).toString();
    keysList.append(key);
    //On instancie une vue descriptiveCard correspondant à la fiche descriptive pour l'objjet sélectionné
    descriptiveCard = new DescriptiveCard(dataManager, mainWindow, this, codeObject, keysList[0],"complete","modify",this);
    descriptiveCard->setWindowFlags(Qt::Dialog);
    descriptiveCard->setAttribute(Qt::WA_DeleteOnClose);
    descriptiveCard->show();
}

void DataViewer::onCreateNewButtonTriggered()
{
    //On instancie une vue descriptiveCard correspondant à la fiche descriptive pour l'objet sélectionné
    descriptiveCard = new DescriptiveCard(dataManager, mainWindow, this, codeObject, keysList[0],"complete","create",this);
    descriptiveCard->setWindowFlags(Qt::Dialog);
    descriptiveCard->setAttribute(Qt::WA_DeleteOnClose);
    descriptiveCard->show();
}

void DataViewer::onCreateCopyButtonTriggered()
{
    //On instancie une vue descriptiveCard correspondant à la fiche descriptive pour l'objet sélectionné
    descriptiveCard = new DescriptiveCard(dataManager, mainWindow, this, codeObject, keysList[0],"complete","copy",this);
    descriptiveCard->setWindowFlags(Qt::Dialog);
    descriptiveCard->setAttribute(Qt::WA_DeleteOnClose);
    descriptiveCard->show();
}

void DataViewer::onEraseButtonTriggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Attention");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("<b>Vous etes sur le point de supprimer un ou plusieurs objets.</b>");
    msgBox.setInformativeText("=> Supprimer definitivement le/les element(s) selectionne(s) ?");
    msgBox.setStandardButtons(QMessageBox::Apply | QMessageBox::Cancel);
    int ret = msgBox.exec();

    switch(ret)
    {
    case QMessageBox::Apply:
        //Cas ou on supprime un/des attribut(s), on doit mettre a jour les colonnes
        if(codeObject == "GAT")
        {
            for(int j=0; j<keysList.count();++j)
            {
                QString nameToDel = dataManager->findValueOfMap("mapGAT", keysList[j], "NomAttribut");
                QString codeObjWhereDel = dataManager->findValueOfMap("mapGAT", keysList[j], "CodeObj");
                dataManager->setColumnToTreatName(nameToDel);
                dataManager->setColumnToTreatCodeObject(codeObjWhereDel);
                QString idConfAttr = dataManager->findValueOfMap("mapGAT", keysList[j], "IdCnfAtt");
                QString nomConfWhereDel = dataManager->findValueOfMap("mapGCA", idConfAttr, "NomConf");
                dataManager->setColumnToTreatConfigName(nomConfWhereDel);
                searchColumnToRemoveIndex();
                dataManager->setSignalChangeColumn(1);
                mainWindow->triggerSignalChangeColumn();
            }
        }

        for(int i=0; i < keysList.count(); ++i)
        {
            dataManager->eraseDataOfMap("map"%codeObject,keysList[i]);
            dataManager->addKeyToMapEraseList(codeObject, keysList[i]);
            mainWindow->setKeysToTreat(keysList[i]);
        }
        dataManager->setIdOfLastSupprObjects(keysList);
        mainWindow->setChoiceAddObject("suppr");
        mainWindow->updateLayoutsViewers();
        mainWindow->updateLayoutsOptions();
        mainWindow->resetKeysToTreat();
        mainWindow->setChoiceAddObject("none");
    }
}

void DataViewer::setColumnHidden()
{
    //On met à jour la map en changeant indicAffichage
    QString columnName = ui->tableView->model()->headerData(index, Qt::Horizontal).toString();
    const QList<QMap <QString, QString> > list = dataManager->getSmallMapsFromMapNameOptions("mapGAT","GAT", codeObject);
    QMap<QString, QString> test;
    QString keyTested;
    const QMap <QString, QMap <QString, QString> > *mapGAT = dataManager->getMapFromName("mapGAT");
    for(int a=0; a<list.count();++a)
    {
        keyTested = list[a].value("key");
        test = mapGAT->value(keyTested);
        if(test.value("NomAttribut") == columnName)
        {
            dataManager->replaceDataOfMap("mapGAT", keyTested, "Non", "IndicAffichage");
            dataManager->addKeyToMapChangeList("mapGAT",keyTested);
            mainWindow->setKeysToTreat(keyTested);
        }
    }

    mainWindow->setChoiceAddObject("modify");
    mainWindow->updateLayoutsViewers();
    mainWindow->updateLayoutsOptions();
    mainWindow->setChoiceAddObject("none");
    mainWindow->removeColumn(codeObject, currentConfigName, index);

    mainWindow->resetKeysToTreat();

}

void DataViewer::onSortContent()
{
    updateKeyRowMap();
}

void DataViewer::onImportButtonTriggered()
{
    importForm = new ImportForm(dataManager, codeObject, this);
    importForm->exec();
}

void DataViewer::onCopyButtonTriggered()
{
    dataManager->setCopiedKeys(keysList);
}

void DataViewer::onExportButtonTriggered()
{;
    exportForm = new ExportForm(dataManager, keysList, codeObject, this);
    exportForm->exec();
}

void DataViewer::onPrintButtonTriggered()
{
    printForm = new PrintForm(ui->tableView,this);
    printForm->exec();
}

void DataViewer::slotUpdateLayout()
{
    updateLayout();
}

void DataViewer::slotChangeColumn()
{
    int signalChangeColumn = dataManager->getSignalChangeColumn();
    QString columnToTreatCodeObject = dataManager->getColumnToTreatCodeObject();
    QString columnToTreatConfigName = dataManager->getColumnToTreatConfigName();

    //Si on a changé l'indicateur d'affichage à "Non" d'une fiche descriptive, ou que l'on supprime un attribut, signalChangeColumn = 1 et on cherche la colonne a enlever
    if(signalChangeColumn == 1)
    {
        searchColumnToRemoveIndex();
        if(columnToTreatCodeObject == codeObject && columnToTreatConfigName == currentConfigName)
        {
            int columnToRemoveIndex = mainWindow->getColumnToRemoveIndex();
            myModel->removeModelColumn(columnToRemoveIndex);
        }
        dataManager->setSignalChangeColumn(0);
    }
    //Si on a changé l'indicateur d'affichage a "Oui" d'une fiche descriptive, signalChangeColumn = 2 et on ajoute la colonne a la suite des autres
    else if(signalChangeColumn == 2)
    {
        if(columnToTreatCodeObject == codeObject && columnToTreatConfigName == currentConfigName)
        {
            int columnToAddIndex = ui->tableView->model()->columnCount();
            QString columnName;
            int columnOfKey;

            //On cherche le numéro de colonne de la clé dans le tableau
            for(int j=0; j<ui->tableView->model()->columnCount();++j)
            {
                columnName = ui->tableView->model()->headerData(j,Qt::Horizontal).toString();
                if(columnName == "key")
                {
                    columnOfKey = j;
                    break;
                }
            }

            const QMap<QString, QMap<QString, QString> >* map = dataManager->getMapFromName("map"%codeObject);
            const QMap<QString, QMap<QString, QString> >* mapGAT = dataManager->getMapFromName("mapGAT");
            QMap<QString, QMap<QString, QString> >::ConstIterator iterator;
            QStringList keysToTreat  = mainWindow->getKeysToTreat();
            iterator = mapGAT->find(keysToTreat[0]);
            QMap<QString, QString> currentTestedMap = mapGAT->value(iterator.key());
            QStringList list;
            const QMap<QString,QString >* mapConcordance = dataManager->getMapConcordance();
            QString codeObj = currentTestedMap["CodeObj"];
            QString numInterne = currentTestedMap["NumeroInterne"];
            QString infoInterne = currentTestedMap["InfoInterne"];

            QString keyOfMapConcordance = codeObj % numInterne % infoInterne;
            QString nameOfAttrToAdd = mapConcordance->value(keyOfMapConcordance);
            QString nameOfColumnToAdd = currentTestedMap["NomAttribut"];

            myModel->addModelColumn(map,columnToAddIndex, columnOfKey, nameOfAttrToAdd, nameOfColumnToAdd);
        }
        dataManager->setSignalChangeColumn(0);
    }
}
QString DataViewer::getCodeObject() const
{
    return codeObject;
}

void DataViewer::setCodeObject(const QString &value)
{
    codeObject = value;
}

QString DataViewer::getCurrentConfigName() const
{
    return currentConfigName;
}

void DataViewer::setCurrentConfigName(const QString &value)
{
    currentConfigName = value;
}


void DataViewer::searchColumnToRemoveIndex()
{
    //On transmet a mainWindow les données obtenues dans dataManager
    QString columToRemoveCodeObj = dataManager->getColumnToTreatCodeObject();
    QString columnToRemoveConfigName = dataManager->getColumnToTreatConfigName();
    mainWindow->setColumnToTreatCodeObject(columToRemoveCodeObj);
    mainWindow->setColumnToTreatConfigName(columnToRemoveConfigName);
    QString columnToRemoveName = dataManager->getColumnToTreatName();
    QString columnName;

    //On cherche l'index de la colonne a supprimer dans la vue
    for(int i=0; i<ui->tableView->model()->columnCount();++i)
    {
        columnName = ui->tableView->model()->headerData(i, Qt::Horizontal).toString();
        if(columnName == columnToRemoveName)
        {
            mainWindow->setColumnToRemoveIndex(i);
            break;
        }
    }
}

void DataViewer::resetModel()
{
    const QList<QMap<QString, QString> > maps = dataManager->getSmallMapsFromMapName("map"%codeObject, codeObject);
    myModel->resetModel(maps);
}

QList<QString> DataViewer::getDisplayedRowsBeforeUpdate() const
{
    return displayedRowsBeforeUpdate;
}

void DataViewer::setDisplayedRowsBeforeUpdate(const QList<QString> &value)
{
    displayedRowsBeforeUpdate = value;
}

QString DataViewer::getIndicSearch() const
{
    return indicSearch;
}

void DataViewer::setIndicSearch(const QString &value)
{
    indicSearch = value;
}

QList<QString> DataViewer::getResultList() const
{
    return resultList;
}

void DataViewer::setResultList(const QList<QString> &value)
{
    resultList = value;
}

int DataViewer::getIndicFirstSearch() const
{
    return indicFirstSearch;
}

void DataViewer::setIndicFirstSearch(int value)
{
    indicFirstSearch = value;
}

QStringList DataViewer::getKeysList() const
{
    return keysList;
}

void DataViewer::setKeysList(const QStringList &value)
{
    keysList = value;
}




