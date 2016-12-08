#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "filereader.h"
#include "datamanager.h"
#include "dataviewer.h"
#include "filewriter.h"
#include "passwordform.h"

#include <QMessageBox>
#include <QDesktopWidget>
#include <QLinearGradient>
#include <QFileDialog>
#include <QStringBuilder>


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
    QSettings *fileSettings = dataManager->getFileSetting();
    fileReader->setFileName(fileSettings->value("fileGVE").toString());
    fileReader->parseFile("Objects_list", "Object", "Id", "mapGVE");
    fileReader->setFileName(fileSettings->value("fileGCA").toString());
    fileReader->parseFile("Objects_list", "Object", "Id", "mapGCA");
    fileReader->setFileName(fileSettings->value("fileGAT").toString());
    fileReader->parseFile("Objects_list", "Object", "Id", "mapGAT");
    fileReader->setFileName(fileSettings->value("fileGRS").toString());
    fileReader->parseFile("Objects_list", "Object", "Id", "mapGRS");
    fileReader->setFileName(fileSettings->value("fileGDO").toString());
    fileReader->parseFile("Objects_list", "Object", "NumOrdre", "mapGDO");
    fileReader->setFileName(fileSettings->value("fileGCS").toString());
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

    QPalette pal(palette());
    QLinearGradient gradient(this->rect().topLeft(), this->rect().bottomRight());
    gradient.setColorAt(0, QColor(255,255,255,255));
    gradient.setColorAt(1, QColor(245,255,255,255));
    pal.setBrush(QPalette::Background, QBrush(gradient));
    this->setPalette(pal);


    //Centre la fenêtre principale sur l'écran
    this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());

    dataManager->setIndicFirstCreate(1);
    dataManager->setIncrementCreation(1);

    dataManager->setAccessLevel(0);
    ui->consultationButton->setChecked(true);

    dataManager->setIndicRestoreState(1);


    //Les features non disponibles actuellement sont grisées
    ui->foldersButton->setEnabled(false);
}

//Destructeur
MainWindow::~MainWindow()
{
    //delete dataViewer;

    QMap<QString, QString> changeList = dataManager->getMapChangeList();
    QMap<QString, QString> addList = dataManager->getMapAddList();
    QMap<QString, QString> eraseList = dataManager->getMapEraseList();
    if(!changeList.values("mapGDO").isEmpty())
    {
        fileWriter->modifyFiles("mapGDO");
    }
    if(!changeList.values("mapGCA").isEmpty())
    {
        fileWriter->modifyFiles("mapGCA");
    }
    if(!changeList.values("mapGAT").isEmpty())
    {
        fileWriter->modifyFiles("mapGAT");
    }
    if(!changeList.values("mapGVE").isEmpty())
    {
        fileWriter->modifyFiles("mapGVE");
    }
    if(!changeList.values("mapGRS").isEmpty())
    {
        fileWriter->modifyFiles("mapGRS");
    }

    if(!addList.values("mapGDO").isEmpty())
    {
        if(!eraseList.values("mapGDO").isEmpty())
        {
            fileWriter->compareCurrentMaps("mapGDO");
            addList = dataManager->getMapAddList();
        }
        if(!addList.values("mapGDO").isEmpty())
        {
            fileWriter->addToFiles("mapGDO");
        }
    }
    if(!addList.values("mapGCA").isEmpty())
    {
        if(!eraseList.values("mapGCA").isEmpty())
        {
            fileWriter->compareCurrentMaps("mapGCA");
            addList = dataManager->getMapAddList();
        }
        if(!addList.values("mapGCA").isEmpty())
        {
            fileWriter->addToFiles("mapGCA");
        }
    }
    if(!addList.values("mapGAT").isEmpty())
    {
        if(!eraseList.values("mapGAT").isEmpty())
        {
            fileWriter->compareCurrentMaps("mapGAT");
            addList = dataManager->getMapAddList();
        }
        if(!addList.values("mapGAT").isEmpty())
        {
            fileWriter->addToFiles("mapGAT");
        }
    }
    if(!addList.values("mapGVE").isEmpty())
    {
        if(!eraseList.values("mapGVE").isEmpty())
        {
            fileWriter->compareCurrentMaps("mapGVE");
            addList = dataManager->getMapAddList();
        }
        if(!addList.values("mapGVE").isEmpty())
        {
            fileWriter->addToFiles("mapGVE");
        }
    }
    if(!addList.values("mapGRS").isEmpty())
    {
        if(!eraseList.values("mapGRS").isEmpty())
        {
            fileWriter->compareCurrentMaps("mapGRS");
            addList = dataManager->getMapAddList();
        }
        if(!addList.values("mapGRS").isEmpty())
        {
            fileWriter->addToFiles("mapGRS");
        }
    }

    if(!eraseList.values("mapGDO").isEmpty())
    {
        fileWriter->eraseOfFiles("mapGDO");
    }
    if(!eraseList.values("mapGCA").isEmpty())
    {
        fileWriter->eraseOfFiles("mapGCA");
    }
    if(!eraseList.values("mapGAT").isEmpty())
    {
        fileWriter->eraseOfFiles("mapGAT");
    }
    if(!eraseList.values("mapGVE").isEmpty())
    {
        fileWriter->eraseOfFiles("mapGVE");
    }
    if(!eraseList.values("mapGRS").isEmpty())
    {
        fileWriter->eraseOfFiles("mapGRS");
    }

    fileWriter->modifyFiles("mapGCS");

    delete dataManager;
    delete fileReader;
    delete ui;
}

void MainWindow::updateLayoutsOptions()
{
    emit this->signalUpdateLayoutsOptions();
}

void MainWindow::updateLayoutsViewers()
{
    emit this->signalUpdateLayoutsViewers();
}

void MainWindow::removeColumn(QString codeObject, QString currentConfigName, int index)
{
    columnToTreatCodeObject = codeObject;
    columnToTreatConfigName = currentConfigName;
    columnToRemoveIndex = index;
    emit this->signalChangeColumn();
}

void MainWindow::triggerSignalChangeColumn()
{
    emit this->signalChangeColumn();
}

void MainWindow::resetKeysToTreat()
{
    keysToTreat.clear();
}


void MainWindow::on_environmentalVariablesButton_released()
{
    //On instancie une vue dataViewer en rentrant les valeurs données par le dataManager
    dataViewer = new DataViewer(dataManager, this, dataManager->getSmallMapsFromMapName("mapGVE", "GVE"),"GVE", this);
    dataViewer->setAttribute(Qt::WA_DeleteOnClose);
    dataViewer->show();

}

void MainWindow::on_attributesConfigurationsButton_released()
{
    //On instancie une vue dataViewer en rentrant les valeurs données par le dataManager
    dataViewer = new DataViewer(dataManager, this, dataManager->getSmallMapsFromMapName("mapGCA", "GCA"), "GCA",  this);
    dataViewer->setAttribute(Qt::WA_DeleteOnClose);
    dataViewer->show();
}

void MainWindow::on_attributesButton_released()
{
    //On instancie une vue dataViewer en rentrant les valeurs données par le dataManager
    dataViewer = new DataViewer(dataManager, this, dataManager->getSmallMapsFromMapName("mapGAT", "GAT"), "GAT", this);
    dataViewer->setAttribute(Qt::WA_DeleteOnClose);
    dataViewer->show();
}

void MainWindow::on_caseSelectionButton_released()
{

    QFileDialog *dialog = new QFileDialog(this);
    dialog->setWindowTitle("Selectionnez le dossier ou se trouvent les fichiers");
    dialog->setFileMode(QFileDialog::Directory);
    dialog->setOption(QFileDialog::ShowDirsOnly);
    if(dialog->exec())
    {
        dataManager->clearAllMaps();

        QStringList directoryNames = dialog->selectedFiles();
        QString directoryName = directoryNames[0];
        QDir *directory = new QDir(directoryName);
        QStringList listOfFiles = directory->entryList();
        if(listOfFiles.contains("GAT_Export.xml") && listOfFiles.contains("GCA_Export.xml") && listOfFiles.contains("GVE_Export.xml") && listOfFiles.contains("GDO_Export.xml") && listOfFiles.contains("GCS_Export.xml"))
        {
            QMessageBox::information(this, "Information", "Le dossier choisi contient les fichiers necessaire au fonctionnement de l'outil");
        }
        else
        {
            QMessageBox::warning(this, "Attention", "Il semble qu'un ou plusieurs fichiers soient manquants pour le bon fonctionnement de l'outil dans le dossier choisi");
        }
        QSettings fileSettings;
        QString name;
        if(listOfFiles.contains("GAT_Export.xml"))
        {
            name = directoryName % "/GAT_Export.xml";
            dataManager->setFileSetting("fileGAT", name);
        }
        if(listOfFiles.contains("GCA_Export.xml"))
        {
            name = directoryName % "/GCA_Export.xml";
            dataManager->setFileSetting("fileGCA", name);
        }
        if(listOfFiles.contains("GVE_Export.xml"))
        {
            name = directoryName % "/GVE_Export.xml";
            dataManager->setFileSetting("fileGVE", name);
        }
        if(listOfFiles.contains("GDO_Export.xml"))
        {
            name = directoryName % "/GDO_Export.xml";;
            dataManager->setFileSetting("fileGDO", name);
        }
        if(listOfFiles.contains("GRS_Export.xml"))
        {
            name = directoryName % "/GRS_Export.xml";
            dataManager->setFileSetting("fileGRS", name);
        }
        if(listOfFiles.contains("GCS_Export.xml"))
        {
            name = directoryName % "/GCS_Export.xml";
            dataManager->setFileSetting("fileGCS", name);
        }

        //On emet le signal que l'affaire a changé et on ferme toutes les fenêtres
        emit signalCaseChanged();

        //On fait appel au fileReader pour parser les fichiers à l'aide de la fonction parseFile()
        fileReader->setFileName(fileSettings.value("fileGVE").toString());
        fileReader->parseFile("Objects_list", "Object", "Id", "mapGVE");
        fileReader->setFileName(fileSettings.value("fileGCA").toString());
        fileReader->parseFile("Objects_list", "Object", "Id", "mapGCA");
        fileReader->setFileName(fileSettings.value("fileGAT").toString());
        fileReader->parseFile("Objects_list", "Object", "Id", "mapGAT");
        fileReader->setFileName(fileSettings.value("fileGRS").toString());
        fileReader->parseFile("Objects_list", "Object", "Id", "mapGRS");
        fileReader->setFileName(fileSettings.value("fileGDO").toString());
        fileReader->parseFile("Objects_list", "Object", "NumOrdre", "mapGDO");
        fileReader->setFileName(fileSettings.value("fileGCS").toString());
        fileReader->parseFile("Objects_list", "Object", "CodeObjet", "mapGCS");

        //On rempli la mapConcordance avec les valeurs du fichier
        dataManager->setDataOfMapConcordance();

        //On définit le nom de la configuration courante par défaut
        QString standardConfigName;
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

        dataManager->setIndicFirstCreate(1);
        dataManager->setIncrementCreation(1);

        dataManager->setAccessLevel(0);
        ui->consultationButton->setChecked(true);

        dataManager->setIndicRestoreState(1);

        QMap<QString, QString> nullMap;

        dataManager->setMapAddList(nullMap);
        dataManager->setMapChangeList(nullMap);
        dataManager->setMapEraseList(nullMap);

    }
}

void MainWindow::on_officialsButton_released()
{
    //On instancie une vue dataViewer en rentrant les valeurs données par le dataManager
    dataViewer = new DataViewer(dataManager, this, dataManager->getSmallMapsFromMapName("mapGRS", "GRS"), "GRS", this);
    dataViewer->setAttribute(Qt::WA_DeleteOnClose);
    dataViewer->show();
}

void MainWindow::on_documentsButton_released()
{
    //On instancie une vue dataViewer en rentrant les valeurs données par le dataManager
    dataViewer = new DataViewer(dataManager, this, dataManager->getSmallMapsFromMapName("mapGDO", "GDO"), "GDO", this);
    dataViewer->setAttribute(Qt::WA_DeleteOnClose);
    dataViewer->show();
}

void MainWindow::on_consultationButton_released()
{
    if(dataManager->getAccessLevel() != 0)
    {
        dataManager->setAccessLevel(0);
    }
    else
    {
        QMessageBox::information(this, "Information", "L'atelier est deja en mode consultation");
    }
}

void MainWindow::on_modificationButton_released()
{
    if(dataManager->getAccessLevel() != 1)
    {
        passwordForm = new PasswordForm(dataManager,1, this);
        passwordForm->setAttribute(Qt::WA_DeleteOnClose);
        passwordForm->exec();
    }
    else
    {
        QMessageBox::information(this, "Information", "L'atelier est deja en mode modification");
    }
}


void MainWindow::on_adminButton_released()
{
    if(dataManager->getAccessLevel() != 2)
    {
        passwordForm = new PasswordForm(dataManager,2, this);
        passwordForm->setAttribute(Qt::WA_DeleteOnClose);
        passwordForm->exec();
    }
    else
    {
        QMessageBox::information(this, "Information", "L'atelier est deja en mode administration");
    }
}
int MainWindow::getColumnToRemoveIndex() const
{
    return columnToRemoveIndex;
}

void MainWindow::setColumnToRemoveIndex(int value)
{
    columnToRemoveIndex = value;
}

QString MainWindow::getColumnToTreatConfigName() const
{
    return columnToTreatConfigName;
}

void MainWindow::setColumnToTreatConfigName(const QString &value)
{
    columnToTreatConfigName = value;
}

QString MainWindow::getColumnToTreatCodeObject() const
{
    return columnToTreatCodeObject;
}

void MainWindow::setColumnToTreatCodeObject(const QString &value)
{
    columnToTreatCodeObject = value;
}


QString MainWindow::getChoiceAddObject() const
{
    return choiceAddObject;
}

void MainWindow::setChoiceAddObject(const QString &value)
{
    choiceAddObject = value;
}

QStringList MainWindow::getKeysToTreat() const
{
    return keysToTreat;
}

void MainWindow::setKeysToTreat(const QString &value)
{
    keysToTreat.append(value);
}

