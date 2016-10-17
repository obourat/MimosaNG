 #include "searchcard.h"
#include "ui_searchcard.h"
#include "datamanager.h"

#include <QLineEdit>
#include <QLabel>
#include <QDebug>
#include <QStringBuilder>
#include <QSpinBox>
#include <QPushButton>
#include <QComboBox>
#include <QTextEdit>
#include <QDate>
#include <QProgressDialog>

SearchCard::SearchCard(DataManager *dataManager, QString codeObject, QString key, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchCard),
    dataManager(dataManager),
    codeObject(codeObject),
    key(key)

{
    ui->setupUi(this);

    QMap<QString,QString> mapsOfKey;
    QMap<QString, QString> ::Iterator iterator;
    QString type;
    QString name;
    QStringList attributesOfExportConfig;
    QStringList attributeNamesOfCurrentConfig;

    ui->label->setText("Recherche par criteres");
    ui->labelInfo->setText("Les criteres non renseignes seront ignores pour la recherche");

    if(codeObject == "GAT")
    {
        this->setWindowTitle("Filtre de recherche d'attribut");
        selectedMap = dataManager->getMapFromName("mapGAT");
        attributesOfExportConfig = dataManager->getAttributesOfExportConfig("GAT");
    }
    else if(codeObject == "GCA")
    {
        this->setWindowTitle("Filtre de recherche de configuration d'attribut");
        selectedMap = dataManager->getMapFromName("mapGCA");
        attributesOfExportConfig = dataManager->getAttributesOfExportConfig("GCA");
    }
    else if(codeObject == "GRS")
    {
        this->setWindowTitle("Fiche descriptive de Responsable");
        selectedMap = dataManager->getMapFromName("mapGRS");
        attributesOfExportConfig = dataManager->getAttributesOfExportConfig("GRS");
    }
    else if(codeObject == "GVE")
    {
        this->setWindowTitle("Fiche descriptive de Variable d'Environnement");
        selectedMap = dataManager->getMapFromName("mapGVE");
        attributesOfExportConfig = dataManager->getAttributesOfExportConfig("GVE");
    }
    else if(codeObject == "GDO")
    {
        this->setWindowTitle("Fiche descriptive de Document");
        selectedMap = dataManager->getMapFromName("mapGDO");
        attributesOfExportConfig = dataManager->getAttributesOfExportConfig("GDO");
    }

    //On selectionne tous les attributs de la clé key
    mapsOfKey = selectedMap->value(key);

    //On cherche tous les noms des attributs la configuration export
    attributeNamesOfCurrentConfig = dataManager->getAttributesOfCurrentConfigNames(attributesOfExportConfig);
    QString testName;
    QString nameAttributeSelected;
    QStringList list;
    QString baliseName;
    const QMap<QString, QMap<QString, QString> > mapGAT = *dataManager->getMapFromName("mapGAT");

    for(iterator = mapsOfKey.begin(); iterator != mapsOfKey.end(); ++iterator)
    {
        name = iterator.key();
        ++iterator;
        type = iterator.value();
        for(int i=0; i!=attributesOfExportConfig.length();i++)
        {
            testName = attributesOfExportConfig[i];

            baliseName = mapGAT[testName]["Titre"];
            list = baliseName.split(" ");
            baliseName = list[0];

            if(baliseName == name)
            {
                nameAttributeSelected = mapGAT[testName]["NomAttribut"];

                setNewWidget(type, name,nameAttributeSelected);
                break;
            }

        }
    }

    QPalette Pal(palette());
    Pal.setColor(QPalette::Window, QColor(255,255,255,240));
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
    this->show();

}



void SearchCard::setNewWidget(QString type, QString name, QString nameAttributeSelected)
{
    if(type == "string")
    {
        QLineEdit *lineEdit = new QLineEdit;
        QLabel *label = new QLabel;
        //lineEdit->setGraphicsEffect();
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        ui->verticalLayout->addWidget(label);
        ui->verticalLayout->addWidget(lineEdit);

    }
    else if(type == "stringF")
    {
        QLineEdit *lineEdit = new QLineEdit;
        QLabel *label = new QLabel;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        ui->verticalLayout->addWidget(label);
        ui->verticalLayout->addWidget(lineEdit);
    }
    else if(type == "num")
    {
        QSpinBox *spinBox = new QSpinBox;
        QLabel *label = new QLabel;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        spinBox->setSpecialValueText(tr("-"));
        ui->verticalLayout->addWidget(label);
        ui->verticalLayout->addWidget(spinBox);
    }
    else if(type == "intF")
    {
        QSpinBox *spinBox = new QSpinBox;
        QLabel *label = new QLabel;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        spinBox->setSpecialValueText(tr("-"));
        ui->verticalLayout->addWidget(label);
        ui->verticalLayout->addWidget(spinBox);
    }
    else if(type == "strdate")
    {
        QLineEdit *lineEdit = new QLineEdit;
        QLabel *label = new QLabel;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        ui->verticalLayout->addWidget(label);
        ui->verticalLayout->addWidget(lineEdit);
    }
    else if(type == "strtime")
    {
        QLineEdit *lineEdit = new QLineEdit;
        QLabel *label = new QLabel;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        ui->verticalLayout->addWidget(label);
        ui->verticalLayout->addWidget(lineEdit);
    }
    else if(type == "time")
    {
        QLineEdit *lineEdit = new QLineEdit;
        QLabel *label = new QLabel;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        ui->verticalLayout->addWidget(label);
        ui->verticalLayout->addWidget(lineEdit);
    }
    else if(type == "bool")
    {
        QComboBox *comboBox = new QComboBox;
        QLabel *label = new QLabel;
        QStringList list = QStringList() << tr("") <<tr("Oui") <<tr("Non");
        comboBox->addItems(list);
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        ui->verticalLayout->addWidget(label);
        ui->verticalLayout->addWidget(comboBox);
    }
    else if(type == "cadrage")
    {
        QComboBox *comboBox = new QComboBox;
        QLabel *label = new QLabel;    QString titre;
        QStringList list = QStringList() << tr("")<< tr("Gauche") <<tr("Droite") <<tr("Centre");
        comboBox->addItems(list);
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        ui->verticalLayout->addWidget(label);
        ui->verticalLayout->addWidget(comboBox);
    }
    else if(type == "format")
    {
        QComboBox *comboBox = new QComboBox;
        QLabel *label = new QLabel;
        QStringList list = QStringList() <<tr("") << tr("L") <<tr("T")<<tr("C");
        comboBox->addItems(list);
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        ui->verticalLayout->addWidget(label);
        ui->verticalLayout->addWidget(comboBox);
    }
    else if(type == "orient")
    {
        QComboBox *comboBox = new QComboBox;
        QLabel *label = new QLabel;
        QStringList list = QStringList() << tr("")<< tr("Paysage") <<tr("Portrait");
        comboBox->addItems(list);
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        ui->verticalLayout->addWidget(label);
        ui->verticalLayout->addWidget(comboBox);
    }
    else if(type == "comment")
    {
        QTextEdit *textEdit = new QTextEdit;
        QLabel *label = new QLabel;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        ui->verticalLayout->addWidget(label);
        ui->verticalLayout->addWidget(textEdit);
    }
    else if(type == "largeur")
    {
        QSpinBox *spinBox = new QSpinBox;
        QLabel *label = new QLabel;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        spinBox->setSpecialValueText(tr("-"));
        ui->verticalLayout->addWidget(label);
        ui->verticalLayout->addWidget(spinBox);
    }
    else
    {
        QLineEdit *lineEdit = new QLineEdit;
        QLabel *label = new QLabel;
        //lineEdit->setGraphicsEffect();
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        ui->verticalLayout->addWidget(label);
        ui->verticalLayout->addWidget(lineEdit);

    }

}






//Fonction de recherche qui listes les clés de la map "map" vérifiant la présence des couples clé, valeur de la map "mapSearch"
QList<QString> SearchCard::searchMatches(const QMap<QString, QMap<QString, QString> >& map, const QMap<QString, QString >& mapSearch)
{
    QString currentMapSearchName;
    QString currentMapSearchValue;
    QString fileValue;
    int iteratorSearchListKeys = 0;
    int iteratorListKeys = 0;
    //Créations de listes contenant les clés des elements recherchés et des objets a traiter
    QList<QString> mapSearchListKeys = mapSearch.keys();
    QList<QString> mapListKeys = map.keys();
    QList<QString> resultList;
    QString currentMapValue;
    QProgressDialog progress("Recherche de resultats...", "Annuler", 0, mapListKeys.size(), this);
    progress.setWindowModality(Qt::WindowModal);

    //Tant que toutes les clés de la map n'ont pas ete traitées
    while(iteratorListKeys != mapListKeys.size())
    {
        if(progress.wasCanceled())
        {
            break;
        }

        //Si il n'y a pas de critère de recherche, on affiche tout
        if(mapSearchListKeys.size() == 0)
        {
            currentMapValue = mapListKeys.value(iteratorListKeys); //valeur de la clé à tester
            resultList.append(currentMapValue);
            ++iteratorListKeys;
            //progress.setValue(iteratorListKeys);
        }
        else
        {
            //Tant que tous les champs de recherches n'ont pas été vérifiés
            while(iteratorSearchListKeys != mapSearchListKeys.size())
            {
                currentMapValue = mapListKeys.value(iteratorListKeys); //valeur de la clé à tester
                currentMapSearchName = mapSearchListKeys.value(iteratorSearchListKeys); //nom du champ à vérifier
                currentMapSearchValue = mapSearch[currentMapSearchName]; //valeur du nom du champ à vérifier

                ++iteratorSearchListKeys;

                fileValue = getFileValue(map, currentMapValue, currentMapSearchName);
                if(fileValue.contains(currentMapSearchValue))
                {
                    if(iteratorSearchListKeys == mapSearchListKeys.size())
                    {
                        resultList.append(currentMapValue);
                        ++iteratorListKeys;
                        iteratorSearchListKeys = 0;
                        break;
                    }
                }
                else
                {
                    iteratorSearchListKeys = 0;
                    ++iteratorListKeys;
                    break;

                }
            }
            progress.setValue(iteratorListKeys);
        }

    }
    progress.setValue(mapListKeys.size());

    return resultList;
}
int SearchCard::getConfirmSearch() const
{
    return confirmSearch;
}

void SearchCard::setConfirmSearch(int value)
{
    confirmSearch = value;
}

QList<QString> SearchCard::getSearchResults() const
{
    return searchResults;
}

void SearchCard::setSearchResults(const QList<QString> &value)
{
    searchResults = value;
}



SearchCard::~SearchCard()
{
    delete ui;
}

void SearchCard::on_buttonBox_accepted()
{
    int lineCount = ui->verticalLayout->count();
    QString valueOfLineEdit;
    QString title;
    int count = (lineCount-1);

    for(int i=0; i<=count;++i)
    {
        QLayoutItem* pLineLabel = ui->verticalLayout->itemAt(i);
        QLabel* pLabel = (QLabel*)pLineLabel->widget();
        title = pLabel->accessibleName();
        ++i;
        QLayoutItem* pLine = ui->verticalLayout->itemAt(i);
        QString widgetType = pLine->widget()->metaObject()->className();

        if(widgetType == "QLineEdit")
        {
            QLineEdit* pLineEdit = (QLineEdit*)pLine->widget();
            valueOfLineEdit = pLineEdit->text();
        }
        else if(widgetType == "QSpinBox")
        {
            QSpinBox* pLineEdit = (QSpinBox*)pLine->widget();
            if(pLineEdit->text() == "-")
            {
                valueOfLineEdit = "";
            }
            else
            {
                valueOfLineEdit = pLineEdit->text();
            }
        }
        else if(widgetType == "QComboBox")
        {
            QComboBox* pLineEdit = (QComboBox*)pLine->widget();
            valueOfLineEdit = pLineEdit->currentText();
        }
        else if(widgetType == "QTextEdit")
        {
            QTextEdit* pLineEdit = (QTextEdit*)pLine->widget();
            valueOfLineEdit = pLineEdit->toPlainText();
        }

        if(valueOfLineEdit != "")
        {
            mapOfSearch.insert(title,valueOfLineEdit);
        }
    }

    searchResults = searchMatches(*selectedMap,mapOfSearch);
    confirmSearch = 1;

    //emit accepted();


}

//Fonction qui retourne la valeur d'une balise de nom "searchName" dans la clé "key", dans la map "map"
QString SearchCard::getFileValue(const QMap<QString, QMap<QString, QString> > map, const QString key, const QString searchName)
{
    QString fileValue = map[key][searchName];
    return fileValue;
}


