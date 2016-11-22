#include "optionsviewer.h"
#include "datamanager.h"
#include "mainwindow.h"
#include "dataviewer.h"
#include "descriptivecard.h"
#include "ui_optionsviewer.h"
#include "model.h"


#include <QSortFilterProxyModel>
#include <QtGui>

OptionsViewer::OptionsViewer(QString codeObject, DataManager *dataManager,MainWindow *mainWindow, DataViewer *dataViewer, const QList<QMap<QString, QString> >& maps, QString selectedOption, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsViewer),
    codeObject(codeObject),
    dataManager(dataManager),
    mainWindow(mainWindow),
    dataViewer(dataViewer),
    selectedOption(selectedOption)

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
        else if(this->codeObject == "GDO")
        {
            currentConfigName = dataManager->getCurrentConfigNameGDO();
            ui->infoNomConf->setText("Nom de la configuration courante : \"" + currentConfigName + "\"  pour le type d'objet : Documents");
        }
        ui->instructionsLabel->setText("Gerez ici les attributs de la configuration courante");

    }
  #if 0
    int columnIteratorMax = columnCount;
    QString columnName;
    int columnNumOrdre;

    for(int j=0; j!= columnIteratorMax; ++j)
    {
        columnName = ui->optionsView->model()->headerData(j,Qt::Horizontal).toString();
        if(columnName == "Numéro d'ordre")
        {
            columnNumOrdre = j;
            break;
        }
    }
  #endif
    connect(ui->optionsView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onItemDoubleClicked()));

    connect(this, SIGNAL(destroyed()), this->parent(), SLOT(updateLayout()));

    QPalette pal(palette());
    QLinearGradient gradient(this->rect().topLeft(), this->rect().bottomRight());
    gradient.setColorAt(0, QColor(255,255,255,255));
    gradient.setColorAt(1, QColor(245,255,255,255));
    pal.setBrush(QPalette::Background, QBrush(gradient));
    this->setPalette(pal);

    connect(mainWindow, SIGNAL(signalUpdateLayoutsOptions()), this, SLOT(slotUpdateLayout()));
}

void OptionsViewer::updateLayout()
{
    const QList<QMap<QString, QString> > maps = dataManager->getSmallMapsFromMapNameOptions("mapGAT","GAT", codeObject);
    QStringList keysToTreat = mainWindow->getKeysToTreat();
    QString choiceAddObject = mainWindow->getChoiceAddObject();
    int columnCount = ui->optionsView->model()->columnCount();

    int columnIteratorMax = columnCount;
    int columnOfKey;
    QString columnName;

    for(int j=0; j!= columnIteratorMax; ++j)
    {
        columnName = ui->optionsView->model()->headerData(j,Qt::Horizontal).toString();
        if(columnName == "key")
        {
            columnOfKey = j;
            break;
        }
    }

    myOptionsModel->updateModelRows(maps,keysToTreat,choiceAddObject,columnOfKey);

    for(int i=0; i<columnCount; ++i)
    {
        columnName = ui->optionsView->model()->headerData(i,Qt::Horizontal).toString();
        if(columnName != "Indicateur d'affichage" && columnName != "Indicateur d'impression" && columnName != "Indicateur de tri" && columnName != "Nom d'attribut" && columnName != "Rang")
        {
            ui->optionsView->setColumnHidden(i, true);
        }
    }

    int rowCount = ui->optionsView->model()->rowCount();
    QString rows = QString::number(rowCount);
    ui->infoNbObject->setText("Nombre d'objets: "+rows);
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

    //On crée le string permettant de stocker la clé de la ligne sélectionnée
    QString key;
    QModelIndexList selectedIndexes = ui->optionsView->selectionModel()->selectedIndexes();

    int columnIteratorMax = columnCount;
    int columnOfKey;
    QString columnName;

    for(int j=0; j!= columnIteratorMax; ++j)
    {
        columnName = ui->optionsView->model()->headerData(j,Qt::Horizontal).toString();
        if(columnName == "key")
        {
            columnOfKey = j;
            break;
        }
    }

    //On soustrait de la liste d'index les index dont la colonne n'est pas égal à la clé
    for(int j=0; j<selectedIndexes.count(); ++j)
    {
        if(selectedIndexes[j].column() != columnOfKey)
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

    if(selectedOption == "attributes")
    {
        QAction* copy = menu->addAction("Copier");
        connect(copy, SIGNAL(triggered()), this, SLOT(onCopyButtonTriggered()));
        QAction* paste = menu->addAction("Coller");
        connect(paste, SIGNAL(triggered()), this, SLOT(onPasteButtonTriggered()));
        if(dataManager->getAccessLevel() < 1)
        {
            paste->setEnabled(false);
        }
    }

    menu->popup(ui->optionsView->viewport()->mapToGlobal(pos));
}

void OptionsViewer::on_confirmButtonBox_accepted()
{
    if(selectedOption == "configurations")
    {
        if(!currentConfigSelectedName.isNull())
        {
            dataManager->setIndicRestoreState(0);

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
            else if(codeObject == "GDO")
            {
                dataManager->setCurrentConfigNameGDO(currentConfigSelectedName);
            }

            dataManager->replaceDataOfMap("mapGCS",codeObject,currentConfigSelectedName,"NomConfStd");

            dataViewer->resetModel();

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
        descriptiveCard = new DescriptiveCard(dataManager, mainWindow, dataViewer, "GCA", keysList[0],"current", "modify",this);
        descriptiveCard->show();
    }
    else if(selectedOption == "attributes")
    {
        //On instancie une vue descriptiveCard correspondant à la fiche descriptive pour l'objet sélectionné
        descriptiveCard = new DescriptiveCard(dataManager, mainWindow, dataViewer, "GAT", keysList[0],"current", "modify",this);
        descriptiveCard->exec();
        updateLayout();
    }
}

void OptionsViewer::onDisplayDescriptiveCardCompleteButtonTriggered()
{
    if(selectedOption == "configurations")
    {
        //On instancie une vue descriptiveCard correspondant à la fiche descriptive pour l'objet sélectionné
        descriptiveCard = new DescriptiveCard(dataManager, mainWindow, dataViewer, "GCA", keysList[0],"complete","modify",this);
        descriptiveCard->show();
    }
    else if(selectedOption == "attributes")
    {
        //On instancie une vue descriptiveCard correspondant à la fiche descriptive pour l'objet sélectionné
        descriptiveCard = new DescriptiveCard(dataManager, mainWindow, dataViewer,"GAT", keysList[0],"complete","modify",this);
        descriptiveCard->exec();
        updateLayout();
    }
}

void OptionsViewer::onCopyButtonTriggered()
{
    dataManager->setCopiedKeys(keysList);
}

void OptionsViewer::onPasteButtonTriggered()
{
    if(codeObject == "GDO")
    {
        QString currentConfigName = dataManager->getCurrentConfigNameGDO();
        idCurrentConfig = dataManager->getCurrentConfigId(currentConfigName,"mapGCA", codeObject);
    }
    else if(codeObject == "GAT")
    {
        QString currentConfigName = dataManager->getCurrentConfigNameGAT();
        idCurrentConfig = dataManager->getCurrentConfigId(currentConfigName,"mapGCA", codeObject);
    }
    else if(codeObject == "GVE")
    {
        QString currentConfigName = dataManager->getCurrentConfigNameGVE();
        idCurrentConfig = dataManager->getCurrentConfigId(currentConfigName,"mapGCA", codeObject);
    }
    else if(codeObject == "GRS")
    {
        QString currentConfigName = dataManager->getCurrentConfigNameGRS();
        idCurrentConfig = dataManager->getCurrentConfigId(currentConfigName,"mapGCA", codeObject);
    }
    else if(codeObject == "GCA")
    {
        QString currentConfigName = dataManager->getCurrentConfigNameGCA();
        idCurrentConfig = dataManager->getCurrentConfigId(currentConfigName,"mapGCA", codeObject);
    }
    dataManager->setNumOrdreMax(ui->optionsView->model()->rowCount());
    dataManager->pasteAttribute(idCurrentConfig, codeObject);
    for(int i=0; i<keysList.count();++i)
    {
        mainWindow->setKeysToTreat(keysList[i]);
    }

    mainWindow->setChoiceAddObject("copy");
    mainWindow->updateLayoutsOptions();
    mainWindow->updateLayoutsViewers();
    mainWindow->setChoiceAddObject("none");

    //On emet le signal qui conduit au slot de changement des colonnes
    mainWindow->triggerSignalChangeColumn();
    mainWindow->resetKeysToTreat();
}

void OptionsViewer::onItemDoubleClicked()
{
    int columnIteratorMax = ui->optionsView->model()->columnCount();
    int columnOfKey;
    QString columnName;

    for(int j=0; j!= columnIteratorMax; ++j)
    {
        columnName = ui->optionsView->model()->headerData(j,Qt::Horizontal).toString();
        if(columnName == "key")
        {
            columnOfKey = j;
            break;
        }
    }
    keysList.clear();
    //On crée la liste d'index qui contient tous les index de toutes les colonnes des lignes sélectionnées
    QModelIndexList selectedIndexes = ui->optionsView->selectionModel()->selectedIndexes();
    QString key;
    key = selectedIndexes[columnOfKey].data(0).toString();
    keysList.append(key);
    //On instancie une vue descriptiveCard correspondant à la fiche descriptive pour l'objjet sélectionné
    if(selectedOption == "configurations")
    {
        //On instancie une vue descriptiveCard correspondant à la fiche descriptive pour l'objet sélectionné
        descriptiveCard = new DescriptiveCard(dataManager, mainWindow, dataViewer, "GCA", keysList[0],"complete","modify",this);
        descriptiveCard->show();
    }
    else if(selectedOption == "attributes")
    {
        //On instancie une vue descriptiveCard correspondant à la fiche descriptive pour l'objet sélectionné
        descriptiveCard = new DescriptiveCard(dataManager, mainWindow, dataViewer,"GAT", keysList[0],"complete","modify",this);
        descriptiveCard->show();
    }
}

void OptionsViewer::slotUpdateLayout()
{
    updateLayout();
}

