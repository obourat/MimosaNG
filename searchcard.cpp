#include "searchcard.h"
#include "ui_searchcard.h"
#include "datamanager.h"

#include <QLineEdit>
#include <QLabel>
#include <QDebug>

SearchCard::SearchCard(DataManager *dataManager, QString codeObject, QString key, QWidget *parent) : dataManager(dataManager), codeObject(codeObject), key(key),
    QDialog(parent),
    ui(new Ui::SearchCard)
{
    ui->setupUi(this);

    QMap<QString,QString> mapsOfKey;
    QMap<QString, QString> ::Iterator iterator;
    QString type;
    QString name;
    QStringList attributesOfCurrentConfig;
    QStringList attributeNamesOfCurrentConfig;

    ui->label->setText("Recherche par criteres");

    if(codeObject == "GAT")
    {
        this->setWindowTitle("Filtre de recherche d'attribut");
        selectedMap = dataManager->getMapFromName("mapGAT");
        attributesOfCurrentConfig = dataManager->getAttributesOfCurrentConfig("GAT");
    }
    else if(codeObject == "GCA")
    {
        this->setWindowTitle("Filtre de recherche de configuration d'attribut");
        selectedMap = dataManager->getMapFromName("mapGCA");
        attributesOfCurrentConfig = dataManager->getAttributesOfCurrentConfig("GCA");
    }
    else if(codeObject == "GRS")
    {
        this->setWindowTitle("Fiche descriptive de Responsable");
        selectedMap = dataManager->getMapFromName("mapGRS");
        attributesOfCurrentConfig = dataManager->getAttributesOfCurrentConfig("GRS");
    }
    else if(codeObject == "GVE")
    {
        this->setWindowTitle("Fiche descriptive de Variable d'Environnement");
        selectedMap = dataManager->getMapFromName("mapGVE");
        attributesOfCurrentConfig = dataManager->getAttributesOfCurrentConfig("GVE");
    }

    mapsOfKey = selectedMap->value(key);

    attributeNamesOfCurrentConfig = dataManager->getAttributesOfCurrentConfigNames(attributesOfCurrentConfig);

    for(iterator = mapsOfKey.begin(); iterator != mapsOfKey.end(); ++iterator)
    {
        name = iterator.key();
        ++iterator;
        type = "string";

        setNewWidget(type, name);

    }

}


void SearchCard::setNewWidget(QString type, QString name)
{
    if(type == "string")
    {
        QLineEdit *lineEdit = new QLineEdit(this);
        QLabel *label = new QLabel(this);
        label->setText(name);

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

    //Tant que toutes les clés de la map n'ont pas ete traitées
    while(iteratorListKeys != mapListKeys.size())
    {
        //Si il n'y a pas de critère de recherche, on affiche tout
        if(mapSearchListKeys.size() == 0)
        {
            currentMapValue = mapListKeys.value(iteratorListKeys); //valeur de la clé à tester
            resultList.append(currentMapValue);
            ++iteratorListKeys;
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
        }




    }

    return resultList;
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
    for(int i=0; i!=lineCount;++i)
    {
        QLayoutItem* pLineLabel = ui->verticalLayout->itemAt(i);
        QLabel* pLabel = (QLabel*)pLineLabel->widget();
        title = pLabel->text();
        ++i;
        QLayoutItem* pLine = ui->verticalLayout->itemAt(i);
        QLineEdit* pLineEdit = (QLineEdit*)pLine->widget();
        valueOfLineEdit = pLineEdit->text();
        if(valueOfLineEdit != "")
        {
            mapOfSearch.insert(title,valueOfLineEdit);
        }
    }

    searchResults = searchMatches(*selectedMap,mapOfSearch);
    //emit accepted();


}

//Fonction qui retourne la valeur d'une balise de nom "searchName" dans la clé "key", dans la map "map"
QString SearchCard::getFileValue(const QMap<QString, QMap<QString, QString> > map, const QString key, const QString searchName)
{
    QString fileValue = map[key][searchName];
    return fileValue;
}


