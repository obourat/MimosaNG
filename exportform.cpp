#include "exportform.h"
#include "ui_exportform.h"
#include "dataviewer.h"
#include "datamanager.h"

#include <QAction>
#include <QFileDialog>
#include <QTextStream>
#include <QStringBuilder>
#include <QMessageBox>

ExportForm::ExportForm(DataManager *dataManager, QStringList keysList, QString codeObjet, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportForm),
    dataManager(dataManager),
    keysList(keysList),
    codeObjet(codeObjet)
{
    ui->setupUi(this);
    this->setWindowTitle("Exporter");
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
    pal.setBrush(QPalette::Window, QBrush(gradient));
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
    int indicFirstValue = 1;

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
                if(indicFirstValue == 1)
                {
                    listOfValueNames.insert(QString::number(i),currentTestedMap.value(baliseName));
                }
                else
                {
                    listOfValueNames.insertMulti(QString::number(i),currentTestedMap.value(baliseName));
                }
            }
            indicFirstValue = 0;
        }
    }

    QFile file(fileName);
    //Cas ou on efface le fichier et réecrit
    if(concatenationChoice == "Non")
    {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        {
            QTextStream stream(&file);

            int maxWidth =0 ;
            QList<int> widthList;
            int numberOfValues = listOfValueNames.values(QString::number(0)).count();

            //On prérentre les largeurs de colonnes minimales à la largeur de leur titre
            int attributeIndex = attributeNames.count() -1;
            for(int ml =0; ml<attributeNames.count();++ml)
            {
                widthList.append(attributeNames[attributeIndex-ml].length());
            }

            //Boucle qui cherche les longueurs maximales de chaque section
            for(int j=0; j<listOfValueNames.count();++j)
            {
                valueNames = listOfValueNames.values(QString::number(j));
                for(int k=0; k<valueNames.count();++k)
                {
                    maxWidth = valueNames[k].length();
                    if(maxWidth > widthList[k])
                    {
                        widthList.replace(k, maxWidth);
                    }
                }
            }

            //Boucle qui remplace les longueurs vides par un espacement minimal
            --numberOfValues;
            for(int l=0; l<widthList.count();++l)
            {
                if(widthList[l] == 0)
                {
                    widthList.replace(l,attributeNames[numberOfValues-l].length());
                }
            }

            //On ecrit dans le fichier
            stream.setFieldAlignment(QTextStream::AlignLeft);
            int attributesCount = attributeNames.count();
            -- attributesCount;
            stream.setFieldAlignment(QTextStream::AlignLeft);
            for(int i= attributesCount; i>=0;--i)
            {
                stream.setFieldWidth(widthList[attributesCount - i] +1);
                stream << attributeNames[i];
            }
            stream.setFieldWidth(0);
            stream << endl;
            for(int j=0; j<listOfValueNames.count();++j)
            {
                valueNames = listOfValueNames.values(QString::number(j));
                for(int k=0; k<valueNames.count();++k)
                {
                    stream.setFieldWidth(widthList[k]+1);
                    stream << valueNames[k];
                }
                stream.setFieldWidth(0);
                stream << endl;
            }
        }
    }

    //Cas ou on ajoute en fin de fichier
    else if(concatenationChoice == "Oui")
    {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        {
            QTextStream stream(&file);

            int maxWidth =0 ;
            QList<int> widthList;
            int numberOfValues = listOfValueNames.values(QString::number(0)).count();

            //On prérentre les largeurs de colonnes minimales à la largeur de leur titre
            int attributeIndex = attributeNames.count() -1;
            for(int ml =0; ml<attributeNames.count();++ml)
            {
                widthList.append(attributeNames[attributeIndex-ml].length());
            }

            //Boucle qui cherche les longueurs maximales de chaque section
            for(int j=0; j<listOfValueNames.count();++j)
            {
                valueNames = listOfValueNames.values(QString::number(j));
                for(int k=0; k<valueNames.count();++k)
                {
                    maxWidth = valueNames[k].length();
                    if(maxWidth > widthList[k])
                    {
                        widthList.replace(k, maxWidth);
                    }
                }
            }

            //Boucle qui remplace les longueurs vides par un espacement minimal
            --numberOfValues;
            for(int l=0; l<widthList.count();++l)
            {
                if(widthList[l] == 0)
                {
                    widthList.replace(l,attributeNames[numberOfValues-l].length());
                }
            }

            //On ecrit dans le fichier
            stream.setFieldAlignment(QTextStream::AlignLeft);
            int attributesCount = attributeNames.count();
            -- attributesCount;
            stream.setFieldAlignment(QTextStream::AlignLeft);
            for(int i= attributesCount; i>=0;--i)
            {
                stream.setFieldWidth(widthList[attributesCount - i] +1);
                stream << attributeNames[i];
            }
            stream.setFieldWidth(0);
            stream << endl;
            for(int j=0; j<listOfValueNames.count();++j)
            {
                valueNames = listOfValueNames.values(QString::number(j));
                for(int k=0; k<valueNames.count();++k)
                {
                    stream.setFieldWidth(widthList[k]+1);
                    stream << valueNames[k];
                }
                stream.setFieldWidth(0);
                stream << endl;
            }
        }
    }
}

void ExportForm::on_browseButton_released()
{
    QFileDialog *dialog = new QFileDialog(this);
    dialog->setWindowTitle("Selectionnez un fichier");
    dialog->setDefaultSuffix("txt");
    dialog->setNameFilter("*.txt");
    if(dialog->exec())
    {
        QStringList fileNames = dialog->selectedFiles();
        QString fileName = fileNames[0];
        ui->fileNameLineEdit->setText(fileName);
    }
}
