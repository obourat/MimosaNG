#include "exportform.h"
#include "ui_exportform.h"
#include "dataviewer.h"
#include "datamanager.h"

#include <QAction>
#include <QFileDialog>
#include <QTextStream>
#include <QStringBuilder>

ExportForm::ExportForm(DataManager *dataManager, QStringList keysList, QString codeObjet, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportForm),
    dataManager(dataManager),
    keysList(keysList),
    codeObjet(codeObjet)
{
    ui->setupUi(this);
    this->setWindowTitle("Options d'exportation");
    ui->destinationComboBox->addItem("Fichier");
    ui->destinationComboBox->addItem("Ecran");
    ui->concatenationComboBox->addItem("Oui");
    ui->concatenationComboBox->addItem("Non");

    configurationsKeys = dataManager->getSmallMapsFromMapNameOptions("mapGCA","GCA", codeObjet);
    QStringList values;
    for(int i=0; i<configurationsKeys.count();++i)
    {
        values = configurationsKeys[i].values();
        ui->configComboBox->addItem(values.at(0));
    }
    QPalette pal(palette());
    QLinearGradient gradient(this->rect().topLeft(), this->rect().bottomRight());
    gradient.setColorAt(0, QColor(255,255,255,255));
    gradient.setColorAt(1, QColor(245,255,255,255));
    pal.setBrush(QPalette::Background, QBrush(gradient));
    this->setPalette(pal);
}

ExportForm::~ExportForm()
{
    delete ui;
}

void ExportForm::on_confirmButtonBox_accepted()
{
    QString destinationChoice = ui->destinationComboBox->currentText();
    QString concatenationChoice = ui->concatenationComboBox->currentText();
    QString fileName = ui->fileNameLineEdit->text();
    QString configurationChoice = ui->configComboBox->currentText();
    QString configKey;

    for(int i=0; i<configurationsKeys.count();++i)
    {
        if(configurationChoice == configurationsKeys[i].values().at(0))
        {
            configKey = configurationsKeys[i].values().at(2);
            break;
        }
    }

    const QMap<QString, QMap<QString, QString> > *mapGAT = dataManager->getMapFromName("mapGAT");
    const QMap<QString, QMap<QString, QString> > *mapSelected = dataManager->getMapFromName("map" % codeObjet);
    const QMap<QString, QString> *mapConcordance = dataManager->getMapConcordance();
    QMap<QString, QMap<QString, QString> >::ConstIterator iterator;
    QMap<QString, QString> attrGATTested;
    QMap<QString, QString> currentTestedMap;
    QStringList attributeNames;
    QStringList valueNames;
    QMap<QString, QString> listOfValueNames;
    QString numeroInterne;
    QString infoInterne;
    QString keyMapConcordance;
    QString baliseName;

    for(iterator = mapGAT->begin(); iterator != mapGAT->end();++iterator)
    {
        attrGATTested = mapGAT->value(iterator.key());
        if(attrGATTested["IdCnfAtt"] == configKey && attrGATTested["IndicAffichage"] == "Oui")
        {
            attributeNames.append(attrGATTested["NomAttribut"]);
            numeroInterne = attrGATTested["NumeroInterne"];
            infoInterne = attrGATTested["InfoInterne"];
            keyMapConcordance = codeObjet % numeroInterne % infoInterne;
            baliseName = mapConcordance->value(keyMapConcordance);

            for(int i=0; i<keysList.count();++i)
            {
                currentTestedMap = mapSelected->value(keysList[i]);
                if(i==0)
                {
                    listOfValueNames.insert(QString::number(i),currentTestedMap.value(baliseName));
                }
                else
                {
                    listOfValueNames.insertMulti(QString::number(i),currentTestedMap.value(baliseName));
                }
                //listOfValueNames[i].append(currentTestedMap.value(baliseName));
            }
        }
    }

    QFile file(fileName);
    if(file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        //stream << fileContent;
    }
}

void ExportForm::on_browseButton_released()
{
    QFileDialog *dialog = new QFileDialog(this);
    dialog->setWindowTitle("Selectionnez un fichier");
    if(dialog->exec())
    {
        QStringList fileNames = dialog->selectedFiles();
        QString fileName = fileNames[0];
        ui->fileNameLineEdit->setText(fileName);
    }
}
