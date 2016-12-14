#include "datamanager.h"

#include <QStringBuilder>
#include <QProgressDialog>
#include <QMessageBox>

DataManager::DataManager()
{
    //On charge les options de noms des fichiers sur lesquels on veut faire le traitement
    fileSetting = new QSettings("filesPaths.cpp", QSettings::IniFormat);
}

DataManager::~DataManager()
{

}

void DataManager::insertDataToMap(const QString &mapName, const QString &key, const QMap<QString, QString> &map)
{
    // Si la clef est vide
    if(key.isEmpty())
    {
        // Quitter
        return;
    }

    // Choix de la map par rapport au nom donné
    if(mapName == "mapGCA")
    {
        // Si la map ne contient pas deja la clef key
        if(!mapGCA.contains(mapName))
        {
            // Insertion des données
            mapGCA.insert(key, map);
        }
    }
    // Choix de la map par rapport au nom donné
    else if(mapName == "mapGAT")
    {
        // Si la map ne contient pas deja la clef key
        if(!mapGAT.contains(mapName))
        {
            // Insertion des données
            mapGAT.insert(key, map);
        }
    }
    // Choix de la map par rapport au nom donné
    else if(mapName == "mapGVE")
    {
        // Si la map ne contient pas deja la clef key
        if(!mapGVE.contains(mapName))
        {
            // Insertion des données
            mapGVE.insert(key, map);
        }
    }
    // Choix de la map par rapport au nom donné
    else if(mapName == "mapGCS")
    {
        // Si la map ne contient pas deja la clef key
        if(!mapGCS.contains(mapName))
        {
            // Insertion des données
            mapGCS.insert(key, map);
        }
    }
    // Choix de la map par rapport au nom donné
    else if(mapName == "mapGRS")
    {
        // Si la map ne contient pas deja la clef key
        if(!mapGRS.contains(mapName))
        {
            // Insertion des données
            mapGRS.insert(key, map);
        }
    }
    // Choix de la map par rapport au nom donné
    else if(mapName == "mapGDO")
    {
        // Si la map ne contient pas deja la clef key
        if(!mapGDO.contains(mapName))
        {
            // Insertion des données
            mapGDO.insert(key, map);
        }
    }
}

void DataManager::eraseDataOfMap(const QString &mapName, const QString &key)
{
    if(mapName == "mapGDO")
    {
        QMap<QString, QMap<QString, QString> >::Iterator iterator;
        for(iterator = mapGDO.begin(); iterator != mapGDO.end(); ++iterator)
        {
            QString keyIterator = iterator.key();
            if(keyIterator == key)
            {
                if(mapEraseList.values(mapName).isEmpty())
                {
                    mapEraseList.insert(mapName, key);
                }
                else
                {
                    mapEraseList.insertMulti(mapName, key);
                }
                mapGDO.erase(iterator);
                break;
            }
        }
    }

    else if(mapName == "mapGAT")
    {
        QMap<QString, QMap<QString, QString> >::Iterator iterator;
        for(iterator = mapGAT.begin(); iterator != mapGAT.end(); ++iterator)
        {
            if(iterator.key() == key)
            {
                if(mapEraseList.values(mapName).isEmpty())
                {
                    mapEraseList.insert(mapName, key);
                }
                else
                {
                    mapEraseList.insertMulti(mapName, key);
                }
                mapGAT.erase(iterator);
                break;
            }
        }
    }

    else if(mapName == "mapGCA")
    {
        QMap<QString, QMap<QString, QString> >::Iterator iterator;
        for(iterator = mapGCA.begin(); iterator != mapGCA.end(); ++iterator)
        {
            if(iterator.key() == key)
            {
                if(mapEraseList.values(mapName).isEmpty())
                {
                    mapEraseList.insert(mapName, key);
                }
                else
                {
                    mapEraseList.insertMulti(mapName, key);
                }
                mapGCA.erase(iterator);
                break;
            }
        }
    }

    else if(mapName == "mapGVE")
    {
        QMap<QString, QMap<QString, QString> >::Iterator iterator;
        for(iterator = mapGVE.begin(); iterator != mapGVE.end(); ++iterator)
        {
            if(iterator.key() == key)
            {
                if(mapEraseList.values(mapName).isEmpty())
                {
                    mapEraseList.insert(mapName, key);
                }
                else
                {
                    mapEraseList.insertMulti(mapName, key);
                }
                mapGVE.erase(iterator);
                break;
            }
        }
    }

    else if(mapName == "mapGRS")
    {
        QMap<QString, QMap<QString, QString> >::Iterator iterator;
        for(iterator = mapGRS.begin(); iterator != mapGRS.end(); ++iterator)
        {
            if(iterator.key() == key)
            {
                if(mapEraseList.values(mapName).isEmpty())
                {
                    mapEraseList.insert(mapName, key);
                }
                else
                {
                    mapEraseList.insertMulti(mapName, key);
                }
                mapGRS.erase(iterator);
                break;
            }
        }
    }
}

void DataManager::replaceDataOfMap(const QString &mapName, const QString &key, const QString &valueToAdd, const QString smallKeyNameToReplace)
{
    // Si la clef est vide
    if(key.isEmpty())
    {
        // Quitter
        return;
    }

    QString type;
    QString var;
    QMap<QString, QString> smallMap;
    QStringList list;

    // Choix de la map par rapport au nom donné
    if(mapName == "mapGCA")
    {
        smallMap = mapGCA[key];
        list = smallMap.values(smallKeyNameToReplace);
        type = list[1];
        var = list[2];

        mapGCA[key].remove(smallKeyNameToReplace);
        mapGCA[key].insert(smallKeyNameToReplace, var);
        mapGCA[key].insertMulti(smallKeyNameToReplace,type);
        mapGCA[key].insertMulti(smallKeyNameToReplace,valueToAdd);
    }
    // Choix de la map par rapport au nom donné
    else if(mapName == "mapGAT")
    {
        smallMap = mapGAT[key];
        list = smallMap.values(smallKeyNameToReplace);
        type = list[1];
        var = list[2];

        mapGAT[key].remove(smallKeyNameToReplace);
        mapGAT[key].insert(smallKeyNameToReplace, var);
        mapGAT[key].insertMulti(smallKeyNameToReplace,type);
        mapGAT[key].insertMulti(smallKeyNameToReplace,valueToAdd);

        //Si on a changé l'indicAffichage, on met a jour le modele de données
        if(smallKeyNameToReplace == "IndicAffichage")
        {

            columnToTreatCodeObject = mapGAT[key].value("CodeObj");
            columnToTreatConfigName = getCurrentConfigNameGAT();
            columnToTreatName = mapGAT[key].value("NomAttribut");
            if( valueToAdd == "Non")
            {
                signalChangeColumn = 1;
            }
            else if (valueToAdd == "Oui")
            {
                signalChangeColumn = 2;
            }
        }
    }
    // Choix de la map par rapport au nom donné
    else if(mapName == "mapGVE")
    {
        smallMap = mapGVE[key];
        list = smallMap.values(smallKeyNameToReplace);
        type = list[1];
        var = list[2];

        mapGVE[key].remove(smallKeyNameToReplace);
        mapGVE[key].insert(smallKeyNameToReplace, var);
        mapGVE[key].insertMulti(smallKeyNameToReplace,type);
        mapGVE[key].insertMulti(smallKeyNameToReplace,valueToAdd);
    }
    // Choix de la map par rapport au nom donné
    else if(mapName == "mapGCS")
    {
        smallMap = mapGCS[key];
        list = smallMap.values(smallKeyNameToReplace);
        type = list[1];

        mapGCS[key].remove(smallKeyNameToReplace);
        mapGCS[key].insert(smallKeyNameToReplace,type);
        mapGCS[key].insertMulti(smallKeyNameToReplace,valueToAdd);
    }
    // Choix de la map par rapport au nom donné
    else if(mapName == "mapGRS")
    {
        smallMap = mapGRS[key];
        list = smallMap.values(smallKeyNameToReplace);
        type = list[1];
        var = list[2];

        mapGRS[key].remove(smallKeyNameToReplace);
        mapGRS[key].insert(smallKeyNameToReplace, var);
        mapGRS[key].insertMulti(smallKeyNameToReplace,type);
        mapGRS[key].insertMulti(smallKeyNameToReplace,valueToAdd);
    }
    // Choix de la map par rapport au nom donné
    else if(mapName == "mapGDO")
    {
        smallMap = mapGDO[key];
        list = smallMap.values(smallKeyNameToReplace);
        type = list[1];
        var = list[2];

        mapGDO[key].remove(smallKeyNameToReplace);
        mapGDO[key].insert(smallKeyNameToReplace, var);
        mapGDO[key].insertMulti(smallKeyNameToReplace,type);
        mapGDO[key].insertMulti(smallKeyNameToReplace,valueToAdd);
    }
}

const QMap<QString, QMap<QString, QString> >* DataManager::getMapFromName(const QString &mapName) const
{
    //Retourne la map correspondante en fonction du nom rentré
    if(mapName == "mapGCA")
    {
        return &mapGCA;
    }

    else if(mapName == "mapGAT")
    {
        return &mapGAT;
    }

    else if(mapName == "mapGVE")
    {
        return &mapGVE;
    }

    else if(mapName == "mapGCS")
    {
        return &mapGCS;
    }

    else if(mapName == "mapGRS")
    {
        return &mapGRS;
    }

    else if(mapName == "mapGDO")
    {
        return &mapGDO;
    }

    //Si un nom invalide ou nul est rentré, on ne renvoie rien
    else
    {
        return NULL;
    }
}

const QMap<QString, QString>* DataManager::getMapConcordance() const
{
    return &mapConcordance;
}

QString DataManager::getCurrentConfigNameGVE() const
{
    return currentConfigNameGVE;
}

void DataManager::setCurrentConfigNameGVE(const QString &value)
{
    currentConfigNameGVE = value;
}

QString DataManager::getCurrentConfigNameGDO() const
{
    return currentConfigNameGDO;
}

void DataManager::setCurrentConfigNameGDO(const QString &value)
{
    currentConfigNameGDO = value;
}

QMap<QString, QString> DataManager::getMapChangeList() const
{
    return mapChangeList;
}

void DataManager::setMapChangeList(const QMap<QString, QString> &value)
{
    mapChangeList = value;
}

QMap<QString, QString> DataManager::getMapAddList()
{
    return mapAddList;
}

void DataManager::setMapAddList(QMap<QString, QString> &value)
{
    mapAddList = value;
}

QMap<QString, QString> DataManager::getMapEraseList()
{
    return mapEraseList;
}

void DataManager::setMapEraseList(QMap<QString, QString> &value)
{
    mapEraseList = value;
}

int DataManager::getIndicFirstCreate() const
{
    return indicFirstCreate;
}

void DataManager::setIndicFirstCreate(int value)
{
    indicFirstCreate = value;
}

int DataManager::getIncrementCreation() const
{
    return incrementCreation;
}

void DataManager::setIncrementCreation(int value)
{
    incrementCreation = value;
}

QString DataManager::getIdOfLastCreatedObject() const
{
    return idOfLastCreatedObject;
}

void DataManager::setIdOfLastCreatedObject(const QString &value)
{
    idOfLastCreatedObject = value;
}

int DataManager::getAccessLevel() const
{
    return accessLevel;
}

void DataManager::setAccessLevel(int value)
{
    accessLevel = value;
}

int DataManager::getIndicRestoreState() const
{
    return indicRestoreState;
}

void DataManager::setIndicRestoreState(int value)
{
    indicRestoreState = value;
}

QStringList DataManager::getCopiedKeys() const
{
    return copiedKeys;
}

void DataManager::setCopiedKeys(const QStringList &value)
{
    copiedKeys = value;
}

int DataManager::getNumOrdreMax() const
{
    return numOrdreMax;
}

void DataManager::setNumOrdreMax(int value)
{
    numOrdreMax = value;
}

QStringList DataManager::getIdOfLastSupprObjects() const
{
    return idOfLastSupprObjects;
}

void DataManager::setIdOfLastSupprObjects(const QStringList &value)
{
    idOfLastSupprObjects = value;
}

QString DataManager::getColumnToTreatCodeObject() const
{
    return columnToTreatCodeObject;
}

void DataManager::setColumnToTreatCodeObject(const QString &value)
{
    columnToTreatCodeObject = value;
}

QString DataManager::getColumnToTreatConfigName() const
{
    return columnToTreatConfigName;
}

void DataManager::setColumnToTreatConfigName(const QString &value)
{
    columnToTreatConfigName = value;
}

QString DataManager::getColumnToTreatName() const
{
    return columnToTreatName;
}

void DataManager::setColumnToTreatName(const QString &value)
{
    columnToTreatName = value;
}

int DataManager::getSignalChangeColumn() const
{
    return signalChangeColumn;
}

void DataManager::setSignalChangeColumn(int value)
{
    signalChangeColumn = value;
}

QSettings *DataManager::getFileSetting() const
{
    return fileSetting;
}

void DataManager::setFileSetting(QString key, QString value)
{
    fileSetting->setValue(key, value);
}

QStringList DataManager::getIdToPaste() const
{
    return idToPaste;
}

void DataManager::setIdToPaste(const QStringList &value)
{
    idToPaste = value;
}


QString DataManager::getCurrentConfigNameGRS() const
{
    return currentConfigNameGRS;
}

void DataManager::setCurrentConfigNameGRS(const QString &value)
{
    currentConfigNameGRS = value;
}

QString DataManager::getCurrentConfigNameGAT() const
{
    return currentConfigNameGAT;
}

void DataManager::setCurrentConfigNameGAT(const QString &value)
{
    currentConfigNameGAT = value;
}

QString DataManager::getCurrentConfigNameGCA() const
{
    return currentConfigNameGCA;
}

void DataManager::setCurrentConfigNameGCA(const QString &value)
{
    currentConfigNameGCA = value;
}

const QStringList DataManager::getAttributesOfCurrentConfig(const QString &objectType)
{
    //On définit une liste de clés keys nous permettant de lister les clés des confs de l'objet objectType
    QStringList keys;

    //On définit un iterateur sur la grande qMap pour passer en revue les clés
    QMap<QString, QMap<QString, QString> >::Iterator iterator;

    //Pour toutes les clés de la qMap GCA
    for (iterator = mapGCA.begin(); iterator != mapGCA.end(); ++iterator)
    {
        //Si l'élément CodeObjet de la clé sélectionnée par l'iterateur est égal au type d'objet rentré
        if(iterator.value()["CodeObjet"] == objectType)
        {
            //On ajoute la clé sélectionnée par l'itérateur dans la liste keys
            keys.append(iterator.key());
        }
    }
    //On définit un string currentKey qui sert a sélectionner la clé pour la configuration courante
    QString currentConfigKey;
    //Pour toutes les clés de la liste keys des clés sélectionnées
    for (int i = 0; i <  keys.length(); ++i)
    {
        //Si la balise NomConf de la clé sélectionnée contient le nom de la configuration courante
        int compareValue;
        if(objectType == "GCA")
        {
            compareValue = QString ::compare(mapGCA[keys[i]]["NomConf"], currentConfigNameGCA, Qt::CaseSensitive);
        }
        else if(objectType == "GAT")
        {
            compareValue = QString ::compare(mapGCA[keys[i]]["NomConf"], currentConfigNameGAT, Qt::CaseSensitive);
        }
        else if(objectType == "GRS")
        {
            compareValue = QString ::compare(mapGCA[keys[i]]["NomConf"], currentConfigNameGRS, Qt::CaseSensitive);
        }
        else if(objectType == "GVE")
        {
            compareValue = QString ::compare(mapGCA[keys[i]]["NomConf"], currentConfigNameGVE, Qt::CaseSensitive);
        }
        else if(objectType == "GDO")
        {
            compareValue = QString ::compare(mapGCA[keys[i]]["NomConf"], currentConfigNameGDO, Qt::CaseSensitive);
        }

        if (compareValue == 0)
        {
            //on sélectionne cette clé comme configuration courante
            currentConfigKey = keys[i];
            //exit
            break;
        }
    }

    //On efface la liste de clés
    keys.clear();
    //Pour chaque clé de la mapGAT
    for (iterator = mapGAT.begin(); iterator != mapGAT.end(); ++iterator)
    {
        //Si la valeur de la balise InCnfAtt est égale à la valeur de la clé de la configuration courante
        if(iterator.value()["IdCnfAtt"] == currentConfigKey)
        {
            //On ajoute la clé sélectionnée par l'iterateur dans la liste keys
            keys.append(iterator.key());
        }
    }
    //On renvoie la liste de clés (correspondant à la liste des clés des attributs pour la configuration d'attribut courante)
    return keys;
}

QStringList DataManager::getAttributesOfCurrentConfigNames(QStringList attributesKeyList)
{
    QString key;
    QString codeObj;
    QString numeroInterne;
    QString infoInterne;
    QString codeTitle;
    QString title;
    QStringList attributesNames;
    for(int i=0;i!=attributesKeyList.length();++i)
    {
        key = attributesKeyList[i];
        codeObj = mapGAT[key]["CodeObj"];
        numeroInterne = mapGAT[key]["NumeroInterne"];
        infoInterne = mapGAT[key]["InfoInterne"];

        codeTitle = codeObj % numeroInterne % infoInterne;
        title = mapConcordance[codeTitle];

        attributesNames.append(title);
    }
    return attributesNames;
}

const QStringList DataManager::getAttributesOfExportConfig(const QString &objectType)
{
    //On définit une liste de clés keys nous permettant de lister les clés des confs de l'objet objectType
    QStringList keys;

    //On définit un iterateur sur la grande qMap pour passer en revue les clés
    QMap<QString, QMap<QString, QString> >::Iterator iterator;

    //Pour toutes les clés de la qMap GCA
    for (iterator = mapGCA.begin(); iterator != mapGCA.end(); ++iterator)
    {
        //Si l'élément CodeObjet de la clé sélectionnée par l'iterateur est égal au type d'objet rentré
        if(iterator.value()["CodeObjet"] == objectType)
        {
            //On ajoute la clé sélectionnée par l'itérateur dans la liste keys
            keys.append(iterator.key());
        }
    }
    //On définit un string currentKey qui sert a sélectionner la clé pour la configuration courante
    QString currentConfigKey;
    //Pour toutes les clés de la liste keys des clés sélectionnées
    for (int i = 0; i <  keys.length(); ++i)
    {
        //Si la balise NomConf de la clé sélectionnée contient le nom de la configuration courante
        int compareValue;
        if(objectType == "GCA")
        {
            compareValue = QString ::compare(mapGCA[keys[i]]["NomConf"], mapGCS["GCA"]["NomConfXML"], Qt::CaseSensitive);
        }
        else if(objectType == "GAT")
        {
            compareValue = QString ::compare(mapGCA[keys[i]]["NomConf"], mapGCS["GAT"]["NomConfXML"], Qt::CaseSensitive);
        }
        else if(objectType == "GRS")
        {
            compareValue = QString ::compare(mapGCA[keys[i]]["NomConf"], mapGCS["GRS"]["NomConfXML"], Qt::CaseSensitive);
        }
        else if(objectType == "GVE")
        {
            compareValue = QString ::compare(mapGCA[keys[i]]["NomConf"], mapGCS["GVE"]["NomConfXML"], Qt::CaseSensitive);
        }
        else if(objectType == "GDO")
        {
            compareValue = QString ::compare(mapGCA[keys[i]]["NomConf"], mapGCS["GDO"]["NomConfXML"], Qt::CaseSensitive);
        }

        if (compareValue == 0)
        {
            //on sélectionne cette clé comme configuration courante
            currentConfigKey = keys[i];
            //exit
            break;
        }
    }

    //On efface la liste de clés
    keys.clear();
    //Pour chaque clé de la mapGAT
    for (iterator = mapGAT.begin(); iterator != mapGAT.end(); ++iterator)
    {
        //Si la valeur de la balise InCnfAtt est égale à la valeur de la clé de la configuration courante
        if(iterator.value()["IdCnfAtt"] == currentConfigKey)
        {
            //On ajoute la clé sélectionnée par l'iterateur dans la liste keys
            keys.append(iterator.key());
        }
    }
    //On renvoie la liste de clés (correspondant à la liste des clés des attributs pour la configuration d'attribut courante)
    return keys;
}

const QList<QMap<QString, QString> > DataManager::getSmallMapsFromMapName(const QString &mapName, QString codeObject)
{
    //On définit une liste de maps<clé, valeur>
    QList<QMap<QString, QString> > maps;
    QList<QMap<QString, QString> > mapsFinal;
    QMap<QString, QString> mapTemp;
    //On définit une map selectedMap qui sera égale à la map sélectionnée
    const QMap<QString, QMap<QString, QString> >* selectedMap;
    //retourne la map correspondante au nom rentré par l'utilisateur (si ce nom est valide)
    selectedMap = getMapFromName(mapName);
    //représente le nom de la clé dans chaque sous map de selectedMap, on différenciera les cas ci besoin
    QString keyName;

    if(codeObject == "GDO")
    {
        keyName = "NumOrdre";
    }
    else
    {
        keyName = "Id";
    }

    //Si la map est nulle
    if(selectedMap == NULL)
    {
        //On retourne une liste vide
        return maps;
    }
    //On définit un iterateur qui parcours les clés de la map sélectionnée
    QMap<QString, QMap<QString, QString> >::ConstIterator iterator;
    //Tant que l'iterateur n'a pas parcouru toutes les clés de la map sélectionnée
    for (iterator= selectedMap->begin(); iterator != selectedMap->end(); ++iterator)
    {
        //On ajoute dans la liste maps les valeurs correspondantes au contenu de la clé (la sous map)
        maps << selectedMap->value(iterator.key());
    }
#if 0
    for(int i=0; i<maps.length();++i)
    {
        mapTemp.clear();
        QMap<QString, QString> elementOfMaps = maps[i];

        QList<QString> valuesList = elementOfMaps.values();
        QList<QString> keysList = elementOfMaps.keys();
        for(int k=0; k != valuesList.length(); ++k)
        {
            valuesList.removeAt(k);
            keysList.removeAt(k);
        }
        for(int l=0; l != valuesList.length(); ++l)
        {
            mapTemp.insert(keysList[l],valuesList[l]);
        }
        mapsFinal << mapTemp;


    }
#endif
    //On sélectionne dans maps les attributs que l'on veut afficher et on retourne le résultat
    return selectAttributesOfSmallMapsList(maps, codeObject, keyName);
}

const QList<QMap<QString, QString> > DataManager::getSmallMapsFromMapNameOptions(const QString &mapName, QString codeObject, QString codeObjectForOptions)
{
    //On définit une liste de maps<clé, valeur>
    QList<QMap<QString, QString> > maps;
    QMap<QString, QString> currentTestedMap;
    //On définit une map selectedMap qui sera égale à la map sélectionnée
    const QMap<QString, QMap<QString, QString> >* selectedMap;
    QString currentConfigName;
    //retourne la map correspondante au nom rentré par l'utilisateur (si ce nom est valide)
    selectedMap = getMapFromName(mapName);
    QString keyName = "Id";

    //Si la map est nulle
    if(selectedMap == NULL)
    {
        //On retourne une liste vide
        return maps;
    }
    //On définit un iterateur qui parcours les clés de la map sélectionnée
    QMap<QString, QMap<QString, QString> >::ConstIterator iterator;
    //Tant que l'iterateur n'a pas parcouru toutes les clés de la map sélectionnée
    for (iterator= selectedMap->begin(); iterator != selectedMap->end(); ++iterator)
    {
        //On définit une map qui est égale à la map de la ligne sélectionnée dans selectedMap
        currentTestedMap = selectedMap->value(iterator.key());
        //Si le codeObjet de la map testée est égal au code objet de l'objet selectionné, on sélectionne l'objet (configuration d'attr ou attr)
        if(mapName == "mapGCA")
        {
            if(currentTestedMap["CodeObjet"] == codeObjectForOptions)
            {
                //On ajoute dans la liste maps les valeurs correpsondantes au contenu de la clé (la sous map)
                maps << selectedMap->value(iterator.key());
            }
        }

        else if(mapName == "mapGAT")
        {
            if(currentTestedMap["CodeObj"] == codeObjectForOptions)
            {
                if(codeObjectForOptions == "GCA")
                {
                    currentConfigName = getCurrentConfigNameGCA();
                }
                else if(codeObjectForOptions == "GAT")
                {
                    currentConfigName = getCurrentConfigNameGAT();
                }
                else if(codeObjectForOptions == "GVE")
                {
                    currentConfigName = getCurrentConfigNameGVE();
                }
                else if(codeObjectForOptions == "GRS")
                {
                    currentConfigName = getCurrentConfigNameGRS();
                }
                else if(codeObjectForOptions == "GDO")
                {
                    currentConfigName = getCurrentConfigNameGDO();
                }


                QString idCurrentCnfAtt = getCurrentConfigId(currentConfigName, "mapGCA", codeObjectForOptions);
                if(currentTestedMap["IdCnfAtt"] == idCurrentCnfAtt )
                {
                    //On ajoute dans la liste maps les valeurs correpsondantes au contenu de la clé (la sous map)
                    maps << selectedMap->value(iterator.key());
                }
            }

        }

    }
    //On sélectionne dans maps les attributs que l'on veut afficher et on retourne le résultat
    return selectAttributesOfSmallMapsListOptions(maps, codeObject, keyName);
}


const QList<QMap<QString, QString> > DataManager::selectAttributesOfSmallMapsList(const QList<QMap<QString, QString> > maps, const QString codeObject, const QString keyName)
{
    QList<QMap<QString, QString> > mapsSelectedForCurrentConfig;
    QStringList attributesOfCurrentConfig = getAttributesOfCurrentConfig(codeObject);
    QMap<QString, QString > mapTempOfSelectedAttributes;
    QMap<QString, QString> valueOfSmallMapSelected;
    QString iteratorAttribute;
    QString nomAttributeDisplayed;
    QString nomAttributeSelected;
    QString valueAttributeSelected;
    QString numeroInterne;
    QString infoInterne;
    QString keyOfmapConcordance;
    QProgressDialog progress("Chargement des donnees...", "Annuler", 0, maps.length());
    progress.setWindowTitle("Chargement");
    progress.setWindowModality(Qt::WindowModal);
    progress.show();

    //Tant que l'on a pas parcouru tous les éléments de maps
    for (int j=0; j < maps.length(); ++j)
    {
        if(progress.wasCanceled())
        {
            break;
        }
        progress.setValue(j);

        mapTempOfSelectedAttributes.clear();
        //Tant que l'on a pas parcouru l'ensemble des clés des attributs sélectionnés pour la conf
        for(int i=0 ; i<attributesOfCurrentConfig.length(); ++i)
        {
            //IteratorAttribute prend en paramètre la clé de l'attribut sur lequel il itère
            iteratorAttribute = attributesOfCurrentConfig.at(i);

            //Si l'indicateur d'affichage de l'attribut séléctionné est égal à "oui"
            if(mapGAT[iteratorAttribute]["IndicAffichage"] == "Oui")
            {
                //On sélectionne le Titre de l'attribut à afficher dans l'en tête
                nomAttributeDisplayed = mapGAT[iteratorAttribute]["NomAttribut"];
                //On selectionne le premier critère NuméroInterne permettant de trouver le nom de la balise à sélectionner
                numeroInterne = mapGAT[iteratorAttribute]["NumeroInterne"];
                //On sélectionne le deuxième critère InfoInterne permettant de trouver le nom de la balise à sélectionner
                infoInterne = mapGAT[iteratorAttribute]["InfoInterne"];

                //On obtient la clé de la valeur à aller chercher dans mapConcordance pour obtenir le nom de la balise sur le XML en concaténant les 3 critères
                keyOfmapConcordance = codeObject % numeroInterne % infoInterne;

                //On cherche la valeur dans mapConcordance
                nomAttributeSelected = mapConcordance[keyOfmapConcordance];
                //On sélectionne l'élément à traiter dans la petite map
                valueOfSmallMapSelected = maps.at(j);
                //On va chercher la valeur dans l'élément de la petite map à la balise nomAttributeSelected
                valueAttributeSelected = valueOfSmallMapSelected[nomAttributeSelected];
                //On insère la valeur dans la map temporaire des éléments sélectionnés
                mapTempOfSelectedAttributes.insert(nomAttributeDisplayed, valueAttributeSelected);

            }
            mapTempOfSelectedAttributes.insert("key", valueOfSmallMapSelected[keyName] );

        }
        //On stocke dans la nouvelle map tous les attributs de la configuration courante
        mapsSelectedForCurrentConfig << mapTempOfSelectedAttributes;
    }
    progress.setValue(maps.length());
    return mapsSelectedForCurrentConfig;
}


const QList<QMap<QString, QString> > DataManager::selectAttributesOfSmallMapsListOptions(const QList<QMap<QString, QString> > maps, const QString codeObject, const QString keyName)
{
    QList<QMap<QString, QString> > mapsSelectedForCurrentConfig;
    QStringList attributesOfCurrentConfig = getAttributesOfExportConfig(codeObject);
    QStringList attributesOfCurrentConfigOptions;
    QMap<QString, QString > mapTempOfSelectedAttributes;
    QMap<QString, QString> valueOfSmallMapSelected;
    QString iteratorAttribute;
    QString nomAttributeDisplayed;
    QString nomAttributeSelected;
    QString valueAttributeSelected;
    QString numeroInterne;
    QString infoInterne;
    QString keyOfmapConcordance;


    for(int k=0 ; k<attributesOfCurrentConfig.length(); ++k)
    {
        //IteratorAttribute prend en paramètre la clé de l'attribut sur lequel il itère
        iteratorAttribute = attributesOfCurrentConfig.at(k);
        if(codeObject == "GCA")
        {
#if 0
            if(mapGAT[iteratorAttribute]["Titre"] == "NomObjet")
            {
                attributesOfCurrentConfigOptions.append(iteratorAttribute);
            }
#endif
            if(mapGAT[iteratorAttribute]["Titre"] == "NomConf")
            {
                attributesOfCurrentConfigOptions.append(iteratorAttribute);
            }

            if(mapGAT[iteratorAttribute]["Titre"] == "Type")
            {
                attributesOfCurrentConfigOptions.append(iteratorAttribute);
            }
        }

        else if(codeObject == "GAT")
        {

            if(mapGAT[iteratorAttribute]["Titre"] == "NomAttribut")
            {
                attributesOfCurrentConfigOptions.append(iteratorAttribute);
            }
            if(mapGAT[iteratorAttribute]["Titre"] == "IndicAffichage")
            {
                attributesOfCurrentConfigOptions.append(iteratorAttribute);
            }
            if(mapGAT[iteratorAttribute]["Titre"] == "IndicImpression")
            {
                attributesOfCurrentConfigOptions.append(iteratorAttribute);
            }
            if(mapGAT[iteratorAttribute]["Titre"] == "IndicTri")
            {
                attributesOfCurrentConfigOptions.append(iteratorAttribute);
            }
        }
    }

    //Tant que l'on a pas parcouru tous les éléments de maps
    for (int j=0; j < maps.length(); ++j)
    {
        mapTempOfSelectedAttributes.clear();
        //Tant que l'on a pas parcouru l'ensemble des clés des attributs sélectionnés pour la conf
        for(int i=0 ; i<attributesOfCurrentConfigOptions.length(); ++i)
        {
            //IteratorAttribute prend en paramètre la clé de l'attribut sur lequel il itère
            iteratorAttribute = attributesOfCurrentConfigOptions.at(i);


                //On sélectionne le Titre de l'attribut à afficher dans l'en tête
                nomAttributeDisplayed = mapGAT[iteratorAttribute]["NomAttribut"];
                //On selectionne le premier critère NuméroInterne permettant de trouver le nom de la balise à sélectionner
                numeroInterne = mapGAT[iteratorAttribute]["NumeroInterne"];
                //On sélectionne le deuxième critère InfoInterne permettant de trouver le nom de la balise à sélectionner
                infoInterne = mapGAT[iteratorAttribute]["InfoInterne"];

                //On obtient la clé de la valeur à aller chercher dans mapConcordance pour obtenir le nom de la balise sur le XML en concaténant les 3 critères
                keyOfmapConcordance = codeObject % numeroInterne % infoInterne;

                //On cherche la valeur dans mapConcordance
                nomAttributeSelected = mapConcordance[keyOfmapConcordance];
                //On sélectionne le l'élément à traiter dans la petite map
                valueOfSmallMapSelected = maps.at(j);
                //On va chercher la valeur dans l'élément de la petite map à la balise nomAttributeSelected
                valueAttributeSelected = valueOfSmallMapSelected[nomAttributeSelected];
                //On insère la valeur dans la map temporaire des éléments sélectionnés
                mapTempOfSelectedAttributes.insert(nomAttributeDisplayed, valueAttributeSelected);


        }
        mapTempOfSelectedAttributes.insert("key", valueOfSmallMapSelected[keyName] );
        //On stocke dans la nouvelle map tous les attributs de la configuration courante
        mapsSelectedForCurrentConfig << mapTempOfSelectedAttributes;
    }

    return mapsSelectedForCurrentConfig;
}

const QString DataManager::getStandardConfigName(QString codeObject)
{
    QString standardConfigName;
    QMap<QString, QString> currentTestedMap;
    //On définit une map selectedMap qui sera égale à la map sélectionnée
    const QMap<QString, QMap<QString, QString> >* selectedMap;
    //retourne la map correspondante au nom rentré par l'utilisateur (si ce nom est valide)
    selectedMap = getMapFromName("mapGCS");

    //Si la map est nulle
    if(selectedMap == NULL)
    {
        //On retourne une liste vide
        return standardConfigName;
    }
    //On définit un iterateur qui parcours les clés de la map sélectionnée
    QMap<QString, QMap<QString, QString> >::ConstIterator iterator;
    //Tant que l'iterateur n'a pas parcouru toutes les clés de la map sélectionnée
    for (iterator= selectedMap->begin(); iterator != selectedMap->end(); ++iterator)
    {
        //On définit une map qui est égale à la map de la ligne sélectionnée dans selectedMap
        currentTestedMap = selectedMap->value(iterator.key());
        //Si le codeObjet de la map testée est égal au code objet de l'objet selectionné, on sélectionne la configuration d'attribut
        if(currentTestedMap["CodeObjet"] == codeObject)
        {
            //On ajoute dans la liste maps les valeurs correpsondantes au contenu de la clé (la sous map)
            standardConfigName = currentTestedMap["NomConfStd"];
            break;
        }
    }
    //On sélectionne dans maps les attributs que l'on veut afficher et on retourne le résultat
    return standardConfigName;
}

const QString DataManager::getCurrentConfigId(const QString currentConfigName, const QString mapName, const QString codeObject)
{
    QString currentConfigId;
    QMap<QString, QString> currentTestedMap;
    //On définit une map selectedMap qui sera égale à la map sélectionnée
    const QMap<QString, QMap<QString, QString> >* selectedMap;
    //retourne la map correspondante au nom rentré par l'utilisateur (si ce nom est valide)
    selectedMap = getMapFromName(mapName);

    //Si la map est nulle
    if(selectedMap == NULL)
    {
        //On retourne une liste vide
        return currentConfigId;
    }
    //On définit un iterateur qui parcours les clés de la map sélectionnée
    QMap<QString, QMap<QString, QString> >::ConstIterator iterator;
    //Tant que l'iterateur n'a pas parcouru toutes les clés de la map sélectionnée
    for (iterator= selectedMap->begin(); iterator != selectedMap->end(); ++iterator)
    {
        //On définit une map qui est égale à la map de la ligne sélectionnée dans selectedMap
        currentTestedMap = selectedMap->value(iterator.key());
        //Si le codeObjet de la map testée est égal au code objet de l'objet selectionné, on sélectionne la configuration d'attribut
        if(currentTestedMap["CodeObjet"] == codeObject )
        {
            if(currentTestedMap["NomConf"] == currentConfigName)
            {
                //On ajoute dans la liste maps les valeurs correpsondantes au contenu de la clé (la sous map)
                currentConfigId = currentTestedMap["Id"];
                break;
            }
        }
    }
    //On sélectionne dans maps les attributs que l'on veut afficher et on retourne le résultat
    return currentConfigId;
}

void DataManager::setDataOfMapConcordance()
{
    QString nameConfXML;
    QString idConf;
    QString codeObject;
    QString numeroInterne;
    QString infoInterne;
    QString titre;
    QString currentKey;
    QString newKey;
    QStringList list;
    //On définit un iterateur sur les grande qMap pour passer en revue les clés
    QMap<QString, QMap<QString, QString> >::Iterator iteratorGCS;
    QMap<QString, QMap<QString, QString> >::Iterator iteratorGCA;
    QMap<QString, QMap<QString, QString> >::Iterator iteratorGAT;

    //Pour chaque éléments de mapGCS
    for(iteratorGCS = mapGCS.begin(); iteratorGCS != mapGCS.end(); ++iteratorGCS)
    {
        //On stocke la clé = au codeObjet
        currentKey = iteratorGCS.key();
        //On prend en paramètre le nom de la conf servant à l'export XML
        nameConfXML = iteratorGCS.value()["NomConfXML"];

        //Pour chaque configuration d'attribut
        for(iteratorGCA = mapGCA.begin(); iteratorGCA != mapGCA.end(); ++iteratorGCA)
        {
            //Si le code objet correspond
            if(iteratorGCA.value()["CodeObjet"] == currentKey)
            {
                //Si il s'agit de la bonne configuration pour le bon code objet
                if(iteratorGCA.value()["NomConf"] == nameConfXML)
                {
                    //On stocke l'Id de la configuration
                    idConf = iteratorGCA.value()["Id"];
                    //Pour chaque attribut de la map d'attribut
                    for(iteratorGAT = mapGAT.begin(); iteratorGAT != mapGAT.end(); ++iteratorGAT)
                    {
                        //Si l'attribut correpond à la bonne configuration
                        if(iteratorGAT.value()["IdCnfAtt"] == idConf)
                        {
                            //On stocke les valeurs des triplés qui une fois concaténés vont nous servir de clés pour accéder au nom de balise
                            codeObject = iteratorGAT.value()["CodeObj"];
                            numeroInterne = iteratorGAT.value()["NumeroInterne"];
                            infoInterne = iteratorGAT.value()["InfoInterne"];

                            titre = iteratorGAT.value()["Titre"];

                                list = titre.split(" ");
                                titre = list[0];

                            newKey = codeObject % numeroInterne % infoInterne;

                            //On insère la valeur dans mapConcordance
                            mapConcordance.insert(newKey, titre);
                        }
                    }
                }
            }
        }
    }
}

void DataManager::addKeyToMapChangeList(QString mapName, QString id)
{
    QString testValue = mapChangeList.value(mapName);
    QStringList mapsOfMapChangeList = mapChangeList.values(mapName);
    int checkExists = 0;
    for(int i=0; i < mapsOfMapChangeList.length(); ++i)
    {
        if(mapsOfMapChangeList[i] == id)
        {
            checkExists = 1;
            break;
        }
    }

    if(checkExists == 0)
    {
        if(testValue.isNull())
        {
            mapChangeList.insert(mapName, id);
        }

        else
        {
            mapChangeList.insertMulti(mapName, id);
        }
    }

}

void DataManager::addKeyToMapAddList(QString mapName, QString id)
{
    QString testValue = mapAddList.value(mapName);
    if(testValue.isNull())
    {
        mapAddList.insert(mapName, id);
    }
    else
    {
        mapAddList.insertMulti(mapName, id);
    }
}

void DataManager::addKeyToMapEraseList(QString mapName, QString id)
{
    QString testValue = mapEraseList.value(mapName);
    if(testValue.isNull())
    {
        mapEraseList.insert(mapName, id);
    }
    else
    {
        mapEraseList.insertMulti(mapName, id);
    }
}

void DataManager::pasteAttribute(QString idCurrentConfig, QString codeObjectPaste)
{
    QMap<QString, QString >::Iterator iteratorId;

    QString currentKey;
    int currentKeyInt;
    int currentMaxKey = 0;
    QList<QString> keysList = mapGAT.keys();

    //On cherche l'id maximal des attributs
    for(int i=0; i<keysList.count(); ++i)
    {
        currentKey = keysList[i];
        currentKeyInt = currentKey.toInt();
        if(currentKeyInt > currentMaxKey)
        {
            currentMaxKey = currentKeyInt;
        }
    }
    currentMaxKey++;

    QMap<QString, QString> mapTemp;
    QStringList tempValues;
    QStringList noCopiedValues;
    int tempValuesCount;
    QString key;
    QString currentMaxKeyStr = QString::number(currentMaxKey);
    QString numOrdreMaxStr;
    QString noCopiedValueName;
    QString info;

    //Pour toutes les clés à copier
    for(int i=0; i<copiedKeys.length(); ++i)
    {
        QString currentKeyToCopy = copiedKeys[i];
        //Si mapGAT contient la clé à copier
        if(mapGAT.contains(currentKeyToCopy))
        {
            //Si le code objet de la clé à copier correspond au code objet du tableau affiché (les objets doivent être de même type), on insert dans la map
            if(mapGAT[currentKeyToCopy].value("CodeObj") == codeObjectPaste)
            {
                QMap<QString, QString> mapId = mapGAT[currentKeyToCopy];
                for(iteratorId = mapId.begin(); iteratorId != mapId.end(); ++iteratorId)
                {
                    key = iteratorId.key();
                    if(key != "Id" && key != "IdCnfAtt" && key != "CodeObj" && key != "Rang")
                    {
                        tempValues = mapId.values(key);
                        tempValuesCount = tempValues.count();
                        --tempValuesCount;
                        for(int j = tempValuesCount; j >=0;--j)
                        {
                            if(mapTemp.values(key).isEmpty())
                            {
                                mapTemp.insert(key, tempValues[j]);
                            }
                            else
                            {
                                mapTemp.insertMulti(key, tempValues[j]);
                            }
                        }
                    }
                    else if(key == "Id")
                    {
                        mapTemp.insert("Id", "-");
                        mapTemp.insertMulti("Id", "string");
                        mapTemp.insertMulti("Id", currentMaxKeyStr);
                    }
                    else if(key == "IdCnfAtt")
                    {
                        mapTemp.insert("IdCnfAtt", "-");
                        mapTemp.insertMulti("IdCnfAtt", "string");
                        mapTemp.insertMulti("IdCnfAtt", idCurrentConfig);
                    }
                    else if(key == "CodeObj")
                    {
                        mapTemp.insert("CodeObj", "-");
                        mapTemp.insertMulti("CodeObj", "string");
                        mapTemp.insertMulti("CodeObj", codeObjectPaste);
                    }
                    else if(key == "Rang")
                    {
                        ++numOrdreMax;
                        mapTemp.insert("Rang", "-");
                        mapTemp.insertMulti("Rang", "string");
                        numOrdreMaxStr = QString::number(numOrdreMax);
                        mapTemp.insertMulti("Rang", numOrdreMaxStr);
                    }
                    ++iteratorId;
                    ++iteratorId;
                }
                mapGAT.insert(currentMaxKeyStr, mapTemp);
                QString mapName = "map"%codeObjectPaste;

                if(mapAddList[mapName].isEmpty())
                {
                    mapAddList.insert(mapName, currentMaxKeyStr);
                }
                else
                {
                    mapAddList.insertMulti(mapName, currentMaxKeyStr);
                }
                //Renseignement de l'id de la nouvelle clé crée
                idToPaste.append(currentMaxKeyStr);

                currentMaxKey++;
                currentMaxKeyStr = QString::number(currentMaxKey);

                mapTemp.clear();
            }
            //Sinon, on ajoute la clé à la liste des objets non copiés
            else
            {
                noCopiedValues.append(currentKeyToCopy);
            }
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Erreur");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("<b>Impossible de coller un element present dans le presse-papier</b>");
            msgBox.setInformativeText("Seuls des attributs valides peuvent etre copies ici");
            msgBox.exec();
        }
    }
    //On indique a l'utilisateur tous les attributs que l'on a pas pu copier
    for(int j=0; j<noCopiedValues.count();++j)
    {
        noCopiedValueName = mapGAT[noCopiedValues[j]].value("NomAttribut");
        info = "L'attribut a copier " + noCopiedValueName + " n'est pas valide.";
        QMessageBox msgBox;
        msgBox.setWindowTitle("Erreur");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(info);
        msgBox.setInformativeText("Selectionnez un autre attribut de meme nom dont le type concorde");
        msgBox.exec();
    }
    noCopiedValues.clear();
}

QString DataManager::findValueOfMap(QString mapName, QString key, QString nameAttribute)
{
    const QMap<QString, QMap<QString, QString> > *map = getMapFromName(mapName);
    QMap<QString, QMap<QString, QString> >::const_iterator iterator;
    iterator = map->find(key);
    const QMap<QString, QString> values = iterator.value();
    QString result = values.value(nameAttribute);
    return result;
}

void DataManager::clearAllMaps()
{
    mapGAT.clear();
    mapGCA.clear();
    mapGCS.clear();
    mapGDO.clear();
    mapGRS.clear();
    mapGVE.clear();
}



