#include "datamanager.h"

#include <QStringBuilder>
#include <QProgressDialog>
#include <QMessageBox>

DataManager::DataManager()
{
    //Chargement des options de noms des fichiers sur lesquels on veut faire le traitement
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
    //Cas d'un objet GDO
    if(mapName == "mapGDO")
    {
        QMap<QString, QMap<QString, QString> >::Iterator iterator;
        //Itération sur mapGDO
        for(iterator = mapGDO.begin(); iterator != mapGDO.end(); ++iterator)
        {
            //Si l'iterateur parcours la bonne clé, suppresion la clé des structures de données et ajout dans la map de suppression
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
                mapGDO.erase(iterator);
                break;
            }
        }
    }
    //Cas d'un objet GAT
    else if(mapName == "mapGAT")
    {
        QMap<QString, QMap<QString, QString> >::Iterator iterator;
        //Itération sur mapGAT
        for(iterator = mapGAT.begin(); iterator != mapGAT.end(); ++iterator)
        {
            //Si l'iterateur parcours la bonne clé, suppresion la clé des structures de données et ajout dans la map de suppression
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
    //Cas d'un objet GCA
    else if(mapName == "mapGCA")
    {
        QMap<QString, QMap<QString, QString> >::Iterator iterator;
        //Itération sur mapGCA
        for(iterator = mapGCA.begin(); iterator != mapGCA.end(); ++iterator)
        {
            //Si l'iterateur parcours la bonne clé, suppresion la clé des structures de données et ajout dans la map de suppression
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
    //Cas d'un objet GVE
    else if(mapName == "mapGVE")
    {
        QMap<QString, QMap<QString, QString> >::Iterator iterator;
        //Itération sur mapGVE
        for(iterator = mapGVE.begin(); iterator != mapGVE.end(); ++iterator)
        {
            //Si l'iterateur parcours la bonne clé, suppresion la clé des structures de données et ajout dans la map de suppression
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
        //Itération sur mapGRS
        for(iterator = mapGRS.begin(); iterator != mapGRS.end(); ++iterator)
        {
            //Si l'iterateur parcours la bonne clé, suppresion la clé des structures de données et ajout dans la map de suppression
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
        //Déletion de la valeur présentes et insertion des nouvelles valeurs (value, type, var)
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
        //Déletion de la valeur présentes et insertion des nouvelles valeurs (value, type, var)
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
        //Déletion de la valeur présentes et insertion des nouvelles valeurs (value, type, var)
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
        //Déletion de la valeur présentes et insertion des nouvelles valeurs (value, type)
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
        //Déletion de la valeur présentes et insertion des nouvelles valeurs (value, type, var)
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
        //Déletion de la valeur présentes et insertion des nouvelles valeurs (value, type, var)
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
    //Définition d'une liste de clés keys permettant de lister les clés des confs de l'objet objectType
    QStringList keys;

    //Définition d'un iterateur sur la grande qMap pour passer en revue les clés
    QMap<QString, QMap<QString, QString> >::Iterator iterator;

    //Pour toutes les clés de la qMap GCA
    for (iterator = mapGCA.begin(); iterator != mapGCA.end(); ++iterator)
    {
        //Si l'élément CodeObjet de la clé sélectionnée par l'iterateur est égal au type d'objet rentré
        if(iterator.value()["CodeObjet"] == objectType)
        {
            //Ajout de la clé sélectionnée par l'itérateur dans la liste keys
            keys.append(iterator.key());
        }
    }
    //Définition d'un string currentKey qui sert a sélectionner la clé pour la configuration courante
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
            //Ajout de la clé sélectionnée par l'iterateur dans la liste keys
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
    //Pour tous les attributs sélectionnés
    for(int i=0;i!=attributesKeyList.length();++i)
    {
        key = attributesKeyList[i];
        codeObj = mapGAT[key]["CodeObj"];
        numeroInterne = mapGAT[key]["NumeroInterne"];
        infoInterne = mapGAT[key]["InfoInterne"];

        codeTitle = codeObj % numeroInterne % infoInterne;
        //Recherche du titre dans mapConcordance
        title = mapConcordance[codeTitle];
        //Ajout du titre à la liste des noms
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
            //Ajout de la clé sélectionnée par l'iterateur dans la liste keys
            keys.append(iterator.key());
        }
    }
    //On renvoie la liste de clés (correspondant à la liste des clés des attributs pour la configuration d'attribut courante)
    return keys;
}

const QList<QMap<QString, QString> > DataManager::getSmallMapsFromMapName(const QString &mapName, QString codeObject)
{
    //Définition d'une liste de maps<clé, valeur>
    QList<QMap<QString, QString> > maps;
    //Définition d'une map selectedMap qui sera égale à la map sélectionnée
    const QMap<QString, QMap<QString, QString> >* selectedMap;
    //Retourne la map correspondante au nom rentré par l'utilisateur (si ce nom est valide)
    selectedMap = getMapFromName(mapName);
    //Représente le nom de la clé dans chaque sous map de selectedMap, on différenciera les cas ci besoin
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
        //Retourne une liste vide
        return maps;
    }
    //Définition d'un iterateur qui parcours les clés de la map sélectionnée
    QMap<QString, QMap<QString, QString> >::ConstIterator iterator;
    //Tant que l'iterateur n'a pas parcouru toutes les clés de la map sélectionnée
    for (iterator= selectedMap->begin(); iterator != selectedMap->end(); ++iterator)
    {
        //Ajout dans la liste maps les valeurs correspondantes au contenu de la clé (la sous map)
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
    //Définition d'une liste de maps<clé, valeur>
    QList<QMap<QString, QString> > maps;
    QMap<QString, QString> currentTestedMap;
    //Définition d'une map selectedMap qui sera égale à la map sélectionnée
    const QMap<QString, QMap<QString, QString> >* selectedMap;
    QString currentConfigName;
    //Retourne la map correspondante au nom rentré par l'utilisateur (si ce nom est valide)
    selectedMap = getMapFromName(mapName);
    QString keyName = "Id";

    //Si la map est nulle
    if(selectedMap == NULL)
    {
        //Retourne une liste vide
        return maps;
    }
    //Définition d'un iterateur qui parcours les clés de la map sélectionnée
    QMap<QString, QMap<QString, QString> >::ConstIterator iterator;
    //Tant que l'iterateur n'a pas parcouru toutes les clés de la map sélectionnée
    for (iterator= selectedMap->begin(); iterator != selectedMap->end(); ++iterator)
    {
        //Définition d'une map qui est égale à la map de la ligne sélectionnée dans selectedMap
        currentTestedMap = selectedMap->value(iterator.key());
        //Si le codeObjet de la map testée est égal au code objet de l'objet selectionné, on sélectionne l'objet (configuration d'attr ou attr)
        if(mapName == "mapGCA")
        {
            if(currentTestedMap["CodeObjet"] == codeObjectForOptions)
            {
                //Ajout dans la liste maps les valeurs correpsondantes au contenu de la clé (la sous map)
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

                //Recherche de l'id de la configuration courante
                QString idCurrentCnfAtt = getCurrentConfigId(currentConfigName, "mapGCA", codeObjectForOptions);
                if(currentTestedMap["IdCnfAtt"] == idCurrentCnfAtt )
                {
                    //Ajout dans la liste maps les valeurs correpsondantes au contenu de la clé (la sous map)
                    maps << selectedMap->value(iterator.key());
                }
            }

        }

    }
    //Sélection dans maps des attributs que l'on veut afficher et retourne le résultat
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
                //Sélection du Titre de l'attribut à afficher dans l'en tête
                nomAttributeDisplayed = mapGAT[iteratorAttribute]["NomAttribut"];
                //Sélection du premier critère NuméroInterne permettant de trouver le nom de la balise à sélectionner
                numeroInterne = mapGAT[iteratorAttribute]["NumeroInterne"];
                //Sélection du deuxième critère InfoInterne permettant de trouver le nom de la balise à sélectionner
                infoInterne = mapGAT[iteratorAttribute]["InfoInterne"];

                //Sélection de la clé de la valeur à aller chercher dans mapConcordance pour obtenir le nom de la balise sur le XML en concaténant les 3 critères
                keyOfmapConcordance = codeObject % numeroInterne % infoInterne;

                //Recherche de la valeur dans mapConcordance
                nomAttributeSelected = mapConcordance[keyOfmapConcordance];
                //Sélection de l'élément à traiter dans la petite map
                valueOfSmallMapSelected = maps.at(j);
                //Recherche de la valeur dans l'élément de la petite map à la balise nomAttributeSelected
                valueAttributeSelected = valueOfSmallMapSelected[nomAttributeSelected];
                //Insertion de la valeur dans la map temporaire des éléments sélectionnés
                mapTempOfSelectedAttributes.insert(nomAttributeDisplayed, valueAttributeSelected);

            }
            mapTempOfSelectedAttributes.insert("key", valueOfSmallMapSelected[keyName] );

        }
        //Stockage dans la nouvelle map tous les attributs de la configuration courante
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
        //Sélection manuelle des attributs dans le cas des options
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
        //Sélection manuelle des attributs dans le cas des options
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


                //Sélection du Titre de l'attribut à afficher dans l'en tête
                nomAttributeDisplayed = mapGAT[iteratorAttribute]["NomAttribut"];
                //Sélection du premier critère NuméroInterne permettant de trouver le nom de la balise à sélectionner
                numeroInterne = mapGAT[iteratorAttribute]["NumeroInterne"];
                //Sélection du deuxième critère InfoInterne permettant de trouver le nom de la balise à sélectionner
                infoInterne = mapGAT[iteratorAttribute]["InfoInterne"];

                //Obtention de la clé de la valeur à aller chercher dans mapConcordance pour obtenir le nom de la balise sur le XML en concaténant les 3 critères
                keyOfmapConcordance = codeObject % numeroInterne % infoInterne;

                //Recherche de la valeur dans mapConcordance
                nomAttributeSelected = mapConcordance[keyOfmapConcordance];
                //Sélection de l'élément à traiter dans la petite map
                valueOfSmallMapSelected = maps.at(j);
                //Recherche de la valeur dans l'élément de la petite map à la balise nomAttributeSelected
                valueAttributeSelected = valueOfSmallMapSelected[nomAttributeSelected];
                //Insertion de la valeur dans la map temporaire des éléments sélectionnés
                mapTempOfSelectedAttributes.insert(nomAttributeDisplayed, valueAttributeSelected);


        }
        mapTempOfSelectedAttributes.insert("key", valueOfSmallMapSelected[keyName] );
        //Stockage dans la nouvelle map tous les attributs de la configuration courante
        mapsSelectedForCurrentConfig << mapTempOfSelectedAttributes;
    }

    return mapsSelectedForCurrentConfig;
}

const QString DataManager::getStandardConfigName(QString codeObject)
{
    QString standardConfigName;
    QMap<QString, QString> currentTestedMap;
    //Définition d'une map selectedMap qui sera égale à la map sélectionnée
    const QMap<QString, QMap<QString, QString> >* selectedMap;
    //Retourne la map correspondante au nom rentré par l'utilisateur (si ce nom est valide)
    selectedMap = getMapFromName("mapGCS");

    //Si la map est nulle
    if(selectedMap == NULL)
    {
        //Retourne une liste vide
        return standardConfigName;
    }
    //Définition d'un iterateur qui parcours les clés de la map sélectionnée
    QMap<QString, QMap<QString, QString> >::ConstIterator iterator;
    //Tant que l'iterateur n'a pas parcouru toutes les clés de la map sélectionnée
    for (iterator= selectedMap->begin(); iterator != selectedMap->end(); ++iterator)
    {
        //Définition d'une map qui est égale à la map de la ligne sélectionnée dans selectedMap
        currentTestedMap = selectedMap->value(iterator.key());
        //Si le codeObjet de la map testée est égal au code objet de l'objet selectionné, on sélectionne la configuration d'attribut
        if(currentTestedMap["CodeObjet"] == codeObject)
        {
            //Ajout dans la liste maps les valeurs correpsondantes au contenu de la clé (la sous map)
            standardConfigName = currentTestedMap["NomConfStd"];
            break;
        }
    }
    //Sélection dans maps des attributs que l'on veut afficher et on retourne le résultat
    return standardConfigName;
}

const QString DataManager::getCurrentConfigId(const QString currentConfigName, const QString mapName, const QString codeObject)
{
    QString currentConfigId;
    QMap<QString, QString> currentTestedMap;
    //Définition d'une map selectedMap qui sera égale à la map sélectionnée
    const QMap<QString, QMap<QString, QString> >* selectedMap;
    //Retourne la map correspondante au nom rentré par l'utilisateur (si ce nom est valide)
    selectedMap = getMapFromName(mapName);

    //Si la map est nulle
    if(selectedMap == NULL)
    {
        //Retourne une liste vide
        return currentConfigId;
    }
    //Définit un iterateur qui parcours les clés de la map sélectionnée
    QMap<QString, QMap<QString, QString> >::ConstIterator iterator;
    //Tant que l'iterateur n'a pas parcouru toutes les clés de la map sélectionnée
    for (iterator= selectedMap->begin(); iterator != selectedMap->end(); ++iterator)
    {
        //Définit une map qui est égale à la map de la ligne sélectionnée dans selectedMap
        currentTestedMap = selectedMap->value(iterator.key());
        //Si le codeObjet de la map testée est égal au code objet de l'objet selectionné, sélection de la configuration d'attribut
        if(currentTestedMap["CodeObjet"] == codeObject )
        {
            if(currentTestedMap["NomConf"] == currentConfigName)
            {
                //Ajout dans la liste maps les valeurs correpsondantes au contenu de la clé (la sous map)
                currentConfigId = currentTestedMap["Id"];
                break;
            }
        }
    }
    //Sélection dans maps des attributs que l'on veut afficher et retourne le résultat
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
    //Définition d'un iterateur sur les grande qMap pour passer en revue les clés
    QMap<QString, QMap<QString, QString> >::Iterator iteratorGCS;
    QMap<QString, QMap<QString, QString> >::Iterator iteratorGCA;
    QMap<QString, QMap<QString, QString> >::Iterator iteratorGAT;

    //Pour chaque éléments de mapGCS
    for(iteratorGCS = mapGCS.begin(); iteratorGCS != mapGCS.end(); ++iteratorGCS)
    {
        //Stockage de la clé = au codeObjet
        currentKey = iteratorGCS.key();
        //Prise en paramètre du nom de la conf servant à l'export XML
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
                    //Stockage de l'Id de la configuration
                    idConf = iteratorGCA.value()["Id"];
                    //Pour chaque attribut de la map d'attribut
                    for(iteratorGAT = mapGAT.begin(); iteratorGAT != mapGAT.end(); ++iteratorGAT)
                    {
                        //Si l'attribut correpond à la bonne configuration
                        if(iteratorGAT.value()["IdCnfAtt"] == idConf)
                        {
                            //Stockage des valeurs des triplés qui une fois concaténés vont servir de clés pour accéder au nom de balise
                            codeObject = iteratorGAT.value()["CodeObj"];
                            numeroInterne = iteratorGAT.value()["NumeroInterne"];
                            infoInterne = iteratorGAT.value()["InfoInterne"];

                            titre = iteratorGAT.value()["Titre"];

                                list = titre.split(" ");
                                titre = list[0];

                            newKey = codeObject % numeroInterne % infoInterne;

                            //Insertion de la valeur dans mapConcordance
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
    //Recherche si la clé est déja inscrite dans mapChangeList
    for(int i=0; i < mapsOfMapChangeList.length(); ++i)
    {
        //Si oui, break
        if(mapsOfMapChangeList[i] == id)
        {
            checkExists = 1;
            break;
        }
    }
    //Sinon, ajout dans la map
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

    //Recherche de l'id maximal des attributs
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
            //Sinon, ajout de la clé à la liste des objets non copiés
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
    //Retourne les valeurs de l'attribut nameAttribute présents dans la map
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



